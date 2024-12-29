/*************************************************************************
 *   Copyright (C) 2009 by Renaud Guezennec                              *
 *                                                                       *
 *   https://rolisteam.org/                                           *
 *                                                                       *
 *   rolisteam is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published   *
 *   by the Free Software Foundation; either version 2 of the License,   *
 *   or (at your option) any later version.                              *
 *                                                                       *
 *   This program is distributed in the hope that it will be useful,     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with this program; if not, write to the                       *
 *   Free Software Foundation, Inc.,                                     *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.           *
 *************************************************************************/
#ifndef JUMPBACKWARDNODE_H
#define JUMPBACKWARDNODE_H

#include "executionnode.h"
#include "result/diceresult.h"
/**
 * @brief The JumpBackwardNode class is dedicated to change the previous dice of one dice option.
 */
class JumpBackwardNode : public ExecutionNode
{
public:
    /**
     * @brief JumpBackwardNode allows to get result from remote node in the execution tree.
     */
    JumpBackwardNode();
    virtual ~JumpBackwardNode();
    /**
     * @brief run - performs the actions
     * @param previous
     */
    virtual void run(ExecutionNode* previous= nullptr) override;

    /**
     * @brief toString
     * @return
     */
    virtual QString toString(bool) const override;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const override;
    /**
     * @brief generateDotTree
     * @param s
     */
    virtual void generateDotTree(QString& s) override;
    /**
     * @brief getCopy
     * @return
     */
    virtual ExecutionNode* getCopy() const override;
    virtual void execute(ExecutionNode* previous= nullptr) override;
private:
    DiceResult* m_diceResult;
    ExecutionNode* m_backwardNode;
};

#endif // JUMPBACKWARDNODE_H
