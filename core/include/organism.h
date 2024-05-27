#include <memory>
#include <vector>
#include "object.h"
#include "organ.h"

class Organism {
public:
	Organism();

	Organism(std::vector<std::unique_ptr<Object>> children, std::vector<std::unique_ptr<Organ>> organs)
		: children(std::move(children)), organs(std::move(organs)) {}

	void addChild(std::unique_ptr<Object> child);
	void addOrgan(std::unique_ptr<Organ> organ);
	void removeChildByIndex(int index);
	void removeOrganByIndex(int index);
	std::vector<std::unique_ptr<Object>>& getChildren();
	std::vector<std::unique_ptr<Organ>>& getOrgans();

private:
	std::vector<std::unique_ptr<Object>> children;
	std::vector<std::unique_ptr<Organ>> organs;
};