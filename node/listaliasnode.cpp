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
#include "listaliasnode.h"

ListAliasNode::ListAliasNode(const QList<DiceAlias*>& apAlias) : m_aliasList(apAlias)
{
    m_result= new StringResult();
}
void ListAliasNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    StringResult* txtResult= dynamic_cast<StringResult*>(m_result);
    txtResult->setHighLight(false);

    txtResult->addText(buildList());
    txtResult->finished();
    if(nullptr != previous)
    {
        // txtResult->setText(previous->getHelp());
        m_result->setPrevious(previous->getResult());
    }

    if(nullptr != m_nextNode)
    {
        m_nextNode->run(this);
    }
}
QString ListAliasNode::buildList() const
{
    QString result(QObject::tr("List of Alias:\n"));
    for(auto& key : m_aliasList)
    {
        result+= QString("%1 : %2  # %3\n").arg(key->pattern(), key->command(), key->comment());
    }
    return result;
}
QString ListAliasNode::toString(bool wl) const
{
    QStringList resultList;
    for(auto& key : m_aliasList)
    {
        resultList << "{" << key->pattern() << key->command() << "}";
    }

    if(wl)
    {
        return QString("%1 [label=\"ListAliasNode %2\"]").arg(m_id, resultList.join(","));
    }
    else
    {
        return m_id;
    }
}
qint64 ListAliasNode::getPriority() const
{
    return 0;
}

ExecutionNode* ListAliasNode::getCopy() const
{
    ListAliasNode* node= new ListAliasNode(m_aliasList);
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
