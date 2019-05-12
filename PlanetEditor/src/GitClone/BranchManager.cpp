#include "pch.h"
#include "BranchManager.h"


BranchManager::BranchManager() {
	//TODO: add loadbranches
	m_branches.push_back(Branch("Master"));
	m_branches.push_back(Branch("potato"));
	m_branches.push_back(Branch("test"));
	m_index = 0;
	m_commandIndex = -1;
	//m_currentBranch = &m_branches[0];
	//m_currentCommand = nullptr;
}
std::vector<const char*> BranchManager::getBranchNames() {
	std::vector<const char*> brNames;
	for (auto& b : m_branches)
		brNames.push_back(b.getName().c_str());
	return brNames;
}

const int BranchManager::getSize() {
	return m_branches.size();
}

const int BranchManager::getIndex() {
	return m_index;
}

Branch& BranchManager::getCurrentBranch() {
	return m_branches[m_index];
}

const bool BranchManager::canMerge() {
	return m_branches[m_index].getParent() && m_branches[m_index].getCommands().size() == 0;
}

const bool BranchManager::createBranch(std::string name, Branch* parent) {
	m_branches.emplace_back(name, parent);
	return true;
}

const bool BranchManager::setBranch(size_t index) {
	if (index < m_branches.size()) {
		m_index = index;
		return true;
	} else
		return false;
}

const bool BranchManager::setBranch(std::string name) {
	for(int i = 0; i < m_branches.size(); i++) {
		if (m_branches[i].getName() == name) {
			m_index = i;
			return true;
		}
	}
	return false;
}

bool BranchManager::addCommand(Tool* tool, Command::Parameters params, std::vector<std::pair<unsigned int, XMFLOAT3>> newPosition) {
	m_commandIndex++;
	m_branches[m_index].addCommand(tool, params, newPosition);
	std::cout << "commandIndex: " << m_commandIndex << std::endl;
	return true;
}

std::vector<std::pair<unsigned int, XMFLOAT3>> BranchManager::undo() {
	if (m_commandIndex >= 0) {
		std::vector<std::pair<unsigned int, XMFLOAT3>> toReturn(m_branches[m_index].getCommands()[m_commandIndex--].newPosition);
		for (auto& pair : toReturn) {
			pair.second.x *= -1;
			pair.second.y *= -1;
			pair.second.z *= -1;
		}
		std::cout << "commandIndex: " << m_commandIndex << std::endl;
		return toReturn;
	}
	std::cout << "commandIndex: " << m_commandIndex << std::endl;
	return std::vector<std::pair<unsigned int, XMFLOAT3>>();
}
std::vector<std::pair<unsigned int, XMFLOAT3>> BranchManager::redo() {
	if (m_commandIndex < (int)m_branches[m_index].getCommands().size()-1) {
		std::cout << "commandIndex: " << m_commandIndex << std::endl;
		return m_branches[m_index].getCommands()[++m_commandIndex].newPosition;
	}
	std::cout << "commandIndex: " << m_commandIndex << std::endl;
	return std::vector<std::pair<unsigned int, XMFLOAT3>>();
}
std::vector<std::pair<unsigned int, XMFLOAT3>> BranchManager::undoTo(size_t index)
{
	return std::vector<std::pair<unsigned int, XMFLOAT3>>();
}
std::vector<std::pair<unsigned int, XMFLOAT3>> BranchManager::redoTo(size_t index)
{
	return std::vector<std::pair<unsigned int, XMFLOAT3>>();
}

void BranchManager::setCurrentCommand(Command& c) {
	m_commandIndex = &c - m_branches[m_index].getCommands().data();
}
void BranchManager::setCurrentCommand(int index) {
	if (index < m_branches[m_index].getCommands().size()) {
		m_commandIndex = index;
	}
}

const bool BranchManager::isCurrentCommand(const Command& c) {
	return &m_branches[m_index].getCommands()[m_commandIndex] == &c;
}

const bool BranchManager::isCurrentCommand(int index) {
	return index == m_commandIndex;
}




