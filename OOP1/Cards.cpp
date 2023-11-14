#include "Cards.h"
#include "Card.h"
#include <algorithm>

void Cards::onIdle(InputManager& input)
{
}

void Cards::onDetecting(InputManager& input)
{
	vector< vector<int> > rows;
	vector< vector<int> > cols;

	continuousList.clear();
	for (int i = 0; i < n_rows * n_cols; i++) {
		auto row = findContinuity(i, 1); // examine rows 
		auto col = findContinuity(i, 2); // examine columns
		if (row.size() >= 3) rows.push_back(row);
		if (col.size() >= 3) cols.push_back(col);
	}
	// find any cross case
	for (auto& row : rows) {
		for (auto& col : cols) {
			if (col.size() % 2 != 0 && row.size() % 2 != 0 && col.size() == row.size() && col[col.size() / 2] == row[row.size() / 2]) { // found matched
				continuousList.insert(row.begin(), row.end());
				continuousList.insert(col.begin(), col.end());
				Debug::Log("found cross %d", continuousList.size());
				break;
			}
		}
		if (continuousList.empty() == false) break;
	}

	if (continuousList.empty() == true) { // if not found any cross case
		for (auto& row : rows) {
			for (auto& col : cols) {
				if (row.size() != col.size()) continue;
				if (row.front() == col.front() || row.front() == col.back() ||
					row.back() == col.front() || row.back() == col.back()) {
					continuousList.insert(row.begin(), row.end());
					continuousList.insert(col.begin(), col.end());
					break;
				}
			}
			if (continuousList.empty() == false) break;
		}
	}
	if (continuousList.empty() == true) {
		size_t max_size = 0;
		vector<int>* max_list = nullptr;
		for (auto& row : rows) {
			if (row.size() > max_size) {
				max_size = row.size();
				max_list = &row;
			}
		}
		for (auto& col : cols) {
			if (col.size() > max_size) {
				max_size = col.size();
				max_list = &col;
			}
		}
		if (max_list != nullptr)
			continuousList.insert(max_list->begin(), max_list->end());
	}

	if (continuousList.size() < 3) {
		state = CardsState::Idle;
		return;
	}

	Debug::Log("# of items = %d", continuousList.size());
	for (auto no : continuousList) {
		Debug::Log(" no = %d", no);
		static_cast<Card*>(cards[no])->setBlinkingPeriod(50);
	}

	n_frames_waiting = 50;
	state = CardsState::Waiting;
}

void Cards::onWaiting(InputManager& input)
{
	if (n_frames_waiting == 0) {
		state = CardsState::Removing;
		return;
	}
	n_frames_waiting--;
}

void Cards::onRemoving(InputManager& input)
{
	for_each(continuousList.begin(), continuousList.end(), 
		[&](int no) { shapes[no] = 0; static_cast<Card*>(cards[no])->setType(shapes[no]); } );
	n_frames_waiting = 20;
	state = CardsState::Assigning;
}

void Cards::onAssigning(InputManager& input)
{
	if (n_frames_waiting == 0) {
		for (auto no : continuousList)
		{
			shapes[no] = randomizeType();
			static_cast<Card*>(cards[no])->setType(shapes[no]);
		}
		state = CardsState::Detecting;
		return;
	}
	
	n_frames_waiting--;
}


void Cards::update(InputManager& input)
{
	(this->*fsm[(int)state])(input);
}
