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
#include "splitnode.h"

SplitNode::SplitNode() : m_diceResult(new DiceResult())
{
    m_result= m_diceResult;
}
void SplitNode::run(ExecutionNode* previous)
{
    if(isValid(!previous, Dice::ERROR_CODE::NO_PREVIOUS_ERROR, tr("No Previous node")))
        return;
    m_previousNode= previous;

    m_result->setPrevious(previous->getResult());

    Result* tmpResult= previous->getResult();
    if(isValid(!tmpResult, Dice::ERROR_CODE::NO_VALID_RESULT, tr("No Valid result")))
        return;

    DiceResult* dice= dynamic_cast<DiceResult*>(tmpResult);
    if(isValid(!dice, Dice::ERROR_CODE::NO_VALID_RESULT, tr("No Valid dice result")))
        return;

    for(auto& oldDie : dice->getResultList())
    {
        oldDie->displayed();
        m_diceResult->setOperator(oldDie->getOp());
        for(qint64& value : oldDie->getListValue())
        {
            Die* tmpdie= new Die();
            tmpdie->insertRollValue(value);
            tmpdie->setBase(oldDie->getBase());
            tmpdie->setMaxValue(oldDie->getMaxValue());
            tmpdie->setValue(value);
            tmpdie->setOp(oldDie->getOp());
            m_diceResult->insertResult(tmpdie);
        }
    }
}

QString SplitNode::toString(bool withLabel) const
{
    if(withLabel)
    {
        return QString("%1 [label=\"SplitNode Node\"]").arg(m_id);
    }
    else
    {
        return m_id;
    }
}
qint64 SplitNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_nextNode)
    {
        priority= m_nextNode->getPriority();
    }
    return priority;
}
ExecutionNode* SplitNode::getCopy() const
{
    SplitNode* node= new SplitNode();
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
