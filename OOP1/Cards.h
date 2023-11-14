#pragma once
#include "GameObject.h"
#include "Card.h"
#include <vector>
#include <set>

using namespace std;

enum class CardsState {
	Idle = 0,
	Detecting,
	Waiting,
	Removing,
	Assigning
};

struct IEvent {
	virtual bool operator()(InputManager& input) = 0;
};


class Cards :
    public GameObject
{
	int n_cols;
	int n_rows;
	Dimension cardDim;
	Card* source;
	Card* destination;

	int* shapes;
	GameObject** cards;

	int n_frames_waiting;

	set<int> continuousList;
	
	CardsState state;

	typedef void (Cards::*onState)(InputManager& input);

	onState fsm[(int)CardsState::Assigning + 1];

	void onIdle(InputManager& input);
	void onDetecting(InputManager& input);
	void onWaiting(InputManager& input);
	void onRemoving(InputManager& input);
	void onAssigning(InputManager& input);

	vector<int> findContinuity(int startingCellNo, int direction) const
	{
		// startingCellNo is assumed to be \in [ 0, n_cols * n_rows )
		vector<int> results;
		int type = shapes[startingCellNo];
		if (type == 0) return results;

		results.push_back(startingCellNo);
		auto mePos = no2pos(startingCellNo);
		auto inc = direction == 1 ? 1 : n_cols;
		for (int next = startingCellNo + inc; 
			next < startingCellNo + (direction == 1 ? 1 : n_rows ) * n_cols; 
			next += inc )
		{
			auto nextPos = no2pos(next);
			if (direction == 1 && nextPos.y != mePos.y) return results; // horizontal 
			if (direction != 1 && nextPos.x != mePos.x) return results; // vertical
			if (type != shapes[next]) return results;
			results.push_back(next);
		}
		return results;
	}

	int randomizeType() const { return  rand() % 4 + 1; }

public:
	Cards(int n_cols, int n_rows, const Dimension& dim) :
		GameObject("", Vector2{ 0, 0 }, { 0, 0 }, false),
		source(nullptr), destination(nullptr),
		n_cols(n_cols), n_rows(n_rows), cardDim(dim),
		shapes(new int[(size_t)n_rows * n_cols]),
		cards(new GameObject* [(size_t)n_rows * n_cols]), n_frames_waiting(0),
		state(CardsState::Detecting),
		fsm(),
		continuousList()
	{
		for (int i = 0; i < n_rows * n_cols; i++)
		{
			shapes[i] = randomizeType();
			cards[i] = new Card(*this, shapes[i], no2pos(i), dim, true);
			auto pos = no2pos(i);
			GameObject::Add(cards[i]);
		}
		fsm[0] = &Cards::onIdle; fsm[1] = &Cards::onDetecting; fsm[2] = &Cards::onWaiting; fsm[3] = &Cards::onRemoving; fsm[4] = &Cards::onAssigning;
	};

	~Cards() {
		delete[] shapes;
		delete[] cards;
	}

	Card* getSource() const { return source; }
	Card* getDestination() const { return destination; }
	void setSource(Card* card) { source = card; }
	void setDestination(Card* card) { destination = card; }

	int pos2no(const Position& pos) const { return (pos.x / cardDim.x) % n_cols + (pos.y / cardDim.y) * n_cols; }
	int pos2no(const Vector2& pos) const { return pos2no(Position{ (int)pos.x, (int)pos.y }); }

	Vector2 no2pos(int no) const { return Vector2{ (no % n_cols) * (float)cardDim.x, (no / n_cols) * (float)cardDim.y }; }

	void swap() {
		if (source == nullptr || destination == nullptr) return;

		auto s_no = source->getNo();
		auto d_no = destination->getNo();

		Debug::Log("swap [%d] <-> [%d]", s_no, d_no);
		
		shapes[s_no] = destination->getType();
		shapes[d_no] = source->getType();
		cards[s_no] = destination;
		destination->setNo(s_no);
		cards[d_no] = source;
		source->setNo(d_no);

		state = CardsState::Detecting;
		source = nullptr;
		destination = nullptr;
	}

	void update(InputManager& input) override;
};

