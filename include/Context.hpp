#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <iostream>
#include "Position.hpp"

using namespace std;

class Context {
    private:
        string displayName;
        // Context parent;
        string parentDisplayName;
        Position parentParentEntryPos;
        Position parentEntryPos;

    public:
        Context() {
            displayName = "";
        }

        Context(string pDisplayName, /*Context pParent*/ string pParentDisplayName = "", Position pParentParentEntryPos = Position(), Position pParentEntryPos = Position()) {
            displayName = pDisplayName;
            parentDisplayName = pParentDisplayName;
            parentParentEntryPos = pParentParentEntryPos;
            parentEntryPos = pParentEntryPos;
        }

        string getDisplayName() { return displayName; }

        Context getParent() {
            Context result;
            result.setDisplayName(parentDisplayName);
            result.setParentEntryPos(parentParentEntryPos);
            return result;
        }

        Position getParentEntryPos() { return parentEntryPos; }

        void setDisplayName(string value) { parentDisplayName = value; }
        void setParentEntryPos(Position pos) { parentEntryPos = pos; }
};

#endif // CONTEXT_HPP
