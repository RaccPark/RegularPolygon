#pragma once
#include "GameObject.h"
class Cards;

enum class CardState {
	Idle,
	Dragging,
	DraggingDone,
};

class Card :
    public GameObject
{
	int			type;
	CardState	state;
	Vector2		offset;
	Vector2		originalPos;

	int			no;

	friend class GameObject;

	Cards&		cardManager;

	int n_frames;

	const char* configureShape(int type) {
		switch (type) {
		case 0: return "                                   ";
		case 1: return "       #    #    #    #    #       ";
		case 2: return "      ###    #   #   #    ###      ";
		case 3: return "      ###    #  ###    #  ###      ";
		case 4: return "      # #  # #  ###    #    #      ";
		case 5: return "      ###  #    ###    #  ###      ";
		}
		return configureShape(0);
	}
	
public:
	Card(Cards& cardManager, int type, const Vector2& pos, const Dimension& dim, bool visible);

	~Card() {}

	void setState(CardState state) { this->state = state; }

	int getNo() const { return no; }
	void setNo(int no);

	int getType() const { return type; }
	void setType(int type) { this->type = type; setShape(configureShape(type)); }

	void update(InputManager& input) override;

	void draw() override;

	void setBlinkingPeriod(int n_frames);
};

