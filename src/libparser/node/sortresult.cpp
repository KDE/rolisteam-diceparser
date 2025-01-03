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
#include "sortresult.h"

#include "die.h"

SortResultNode::SortResultNode() : m_diceResult(new DiceResult)
{
    m_ascending= true;
    m_result= m_diceResult;
}
void SortResultNode::run(ExecutionNode* previous)
{
    if(isValid(!previous, Dice::ERROR_CODE::NO_PREVIOUS_ERROR, tr("No Previous node")))
        return;
    m_previousNode= previous;

    DiceResult* previousDiceResult= dynamic_cast<DiceResult*>(previous->getResult());
    m_diceResult->setPrevious(previousDiceResult);
    if(isValid(!previousDiceResult, Dice::ERROR_CODE::NO_VALID_RESULT, tr("No Valid dice result")))
        return;

    auto const& diceList= previousDiceResult->getResultList();
    QList<Die*> diceList2= m_diceResult->getResultList();

    // half-interval search sorting
    for(int i= 0; i < diceList.size(); ++i)
    {
        Die* tmp1= new Die(*diceList[i]);
        // qDebug() << tmp1->getColor() << diceList[i]->getColor();
        //*tmp1=*diceList[i];
        diceList[i]->displayed();

        int j= 0;
        bool found= false;
        int start= 0;
        int end= diceList2.size();
        Die* tmp2= nullptr;
        while(!found)
        {
            int distance= end - start;
            j= (start + end) / 2;
            if(distance == 0)
            {
                j= end;
                found= true;
            }
            else
            {
                tmp2= diceList2[j];
                if(tmp1->getValue() < tmp2->getValue())
                {
                    end= j;
                }
                else
                {
                    start= j + 1;
                }
            }
        }
        diceList2.insert(j, tmp1);
    }

    if(!m_ascending)
    {
        for(int i= 0; i < diceList2.size() / 2; ++i)
        {
            diceList2.swapItemsAt(i, diceList2.size() - (1 + i));
        }
    }
    m_diceResult->setResultList(diceList2);
}
void SortResultNode::setSortAscending(bool asc)
{
    m_ascending= asc;
}
QString SortResultNode::toString(bool wl) const
{
    if(wl)
    {
        auto order= m_ascending ? QStringLiteral("Ascending") : QStringLiteral("Descending");
        return QString("%1 [label=\"SortResultNode %2\"]").arg(m_id, order);
    }
    else
    {
        return m_id;
    }
}
qint64 SortResultNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_previousNode)
    {
        priority= m_previousNode->getPriority();
    }
    return priority;
}
ExecutionNode* SortResultNode::getCopy() const
{
    SortResultNode* node= new SortResultNode();
    node->setSortAscending(m_ascending);
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
