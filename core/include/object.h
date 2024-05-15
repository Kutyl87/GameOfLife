#include <array>
#include <memory>

class Object {
public:
	Object(std::array<float, 3> position, std::array<float, 3> rotation, std::weak_ptr<Object> parent)
		: position(position), rotation(rotation), parent(parent) {}

	std::array<float, 3> getPosition() const;
	std::array<float, 3> getRotation() const;
	std::weak_ptr<Object> getParent() const;
	void setPosition(std::array<float, 3> position);
	void setRotation(std::array<float, 3> rotation);
	void setParent(std::weak_ptr<Object> parent);

private:
	std::array<float, 3> position;
	std::array<float, 3> rotation;
	std::weak_ptr<Object> parent;
};