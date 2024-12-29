/***************************************************************************
 * Copyright (C) 2014 by Renaud Guezennec                                   *
 * https://rolisteam.org/contact                      *
 *                                                                          *
 *  This file is part of DiceParser                                         *
 *                                                                          *
 * DiceParser is free software; you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation; either version 2 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * This program is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with this program; if not, write to the                            *
 * Free Software Foundation, Inc.,                                          *
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.                 *
 ***************************************************************************/
#ifndef BINDNODE_H
#define BINDNODE_H

#include "node/executionnode.h"
#include "result/diceresult.h"

/**
 * @brief The MergeNode class is an ExecutionNode. It is dedicated to merge result of several commands.
 */
class BindNode : public ExecutionNode
{
public:
    BindNode();
    void run(ExecutionNode* previous);
    virtual QString toString(bool withLabel) const;
    virtual qint64 getPriority() const;
    virtual ExecutionNode* getCopy() const;
    std::vector<ExecutionNode*>* getStartList() const;
    void setStartList(std::vector<ExecutionNode*>* startList);

private:
    ExecutionNode* getLatestNode(ExecutionNode* node);

private:
    DiceResult* m_diceResult;
    std::vector<ExecutionNode*>* m_startList{nullptr};
};

#endif // NUMBERNODE_H
