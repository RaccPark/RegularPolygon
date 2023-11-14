#include "Card.h"
#include "Cards.h"
#include "InputManager.h"

Card::Card(Cards& cardManager, int type, const Vector2& pos, const Dimension& dim, bool visible)
	: GameObject("", pos, dim, visible), cardManager(cardManager), state(CardState::Idle), offset(0, 0), originalPos(pos), type(type), no(-1), n_frames(0)
{
	setNo(cardManager.pos2no(pos));
	//Debug::Log("card[%2d] type[%d], loc[%d, %d]", no, type, (int)pos.x, (int)pos.y );
	setShape(configureShape(type));
}

void Card::setNo(int no) { this->no = no; setPosition(cardManager.no2pos(no)); }

void Card::draw() {
	if (n_frames > 0) {
		n_frames--;
		if (n_frames % 2 == 0) return;
	}
	GameObject::draw();
}

void Card::setBlinkingPeriod(int n_frames)
{
	this->n_frames = n_frames;
}

void Card::update(InputManager& input)
{
	Position mousePos;
	Vector2 pos, myPos;
	Card* source = nullptr;
	Card* destination = nullptr;

	if (input.getMouseButtonDown(1)) {
		mousePos = input.getMousePosition();
		pos = mousePos;
		myPos = getPosition();
		if (isInside(pos)) {
			Debug::Log("card[%d] info type(%d) ", getNo(), getType());
		}
	}

	switch (state)
	{
	case CardState::Idle:
		if (input.getMouseButtonDown(0)) { // dragging start
			mousePos = input.getMousePosition();
			pos = mousePos;
			myPos = getPosition();
			if (!isInside(pos)) break;
			source = cardManager.getSource();
			if (source != nullptr) break;
			offset = pos;
			cardManager.setSource(this);
			originalPos = getPosition();
			state = CardState::Dragging;
		}
		if (input.getMouseButtonUp(0)) {
			source = cardManager.getSource();
			destination = cardManager.getDestination();
			if (this == source) break;
			if (destination != nullptr) break;
			mousePos = input.getMousePosition();
			pos = mousePos;
			if (!isInside(pos)) break;
			cardManager.setDestination(this);
		}
		break;
	case CardState::Dragging:
		if (input.getMouseButtonUp(0)) { // dropping
			source = cardManager.getSource();
			if (this != source) break;
			setState(CardState::DraggingDone);
			break;
		}
		mousePos = input.getMousePosition();
		pos = getPosition();
		setPosition(pos + mousePos - offset);
		offset = mousePos;
		break;
	case CardState::DraggingDone:
		destination = cardManager.getDestination();
		source = cardManager.getSource();
		if (destination == nullptr) { 
			setPosition(originalPos);
			setState(CardState::Idle);
			cardManager.setSource(nullptr);
			break;
		}
		pos = destination->getPosition();
		destination->setPosition(originalPos);
		source->setPosition(pos);
		source->setState(CardState::Idle);
		originalPos = source->getPosition();

		cardManager.swap();
		break;
	}

}