#include "dicerollernode.h"
#include "die.h"

#include <QDebug>
#include <QThread>
#include <QThreadPool>
#include <QTime>

DiceRollerNode::DiceRollerNode(qint64 max, qint64 min)
    : m_max(max), m_diceResult(new DiceResult()), m_min(min), m_operator(Dice::ArithmeticOperator::PLUS)
{
    m_result= m_diceResult;
}
void DiceRollerNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    if(isValid(!previous, Dice::ERROR_CODE::NO_PREVIOUS_ERROR, QObject::tr("No Previous node")))
        return;

    Result* result= previous->getResult();

    if(isValid(!result, Dice::ERROR_CODE::NO_VALID_RESULT, QObject::tr("No result from previous node")))
        return;

    auto num= result->getResult(Dice::RESULT_TYPE::SCALAR).toReal();

    isValid(num <= 0, Dice::ERROR_CODE::NO_DICE_TO_ROLL, QObject::tr("No dice to roll"));

    m_diceCount= num > 0 ? static_cast<quint64>(num) : 0;
    m_result->setPrevious(result);

    auto possibleValue= static_cast<quint64>(std::abs((m_max - m_min) + 1));
    if(isValid(possibleValue < m_diceCount && m_unique, Dice::ERROR_CODE::TOO_MANY_DICE,
               QObject::tr("More unique values asked than possible values (D operator)")))
        return;

    for(quint64 i= 0; i < m_diceCount; ++i)
    {
        Die* die= new Die();
        die->setOp(m_operator);
        die->setBase(m_min);
        die->setMaxValue(m_max);
        die->roll();
        if(m_unique)
        {
            const auto& equal= [](const Die* a, const Die* b) { return a->getValue() == b->getValue(); };
            while(m_diceResult->contains(die, equal))
            {
                die->roll(false);
            }
        }
        m_diceResult->insertResult(die);
    }
    /*if(nullptr != m_nextNode)
    {TODO nextNode to null?
        m_nextNode->run(this);
    }*/
}

quint64 DiceRollerNode::getFaces() const
{
    return static_cast<quint64>(std::abs(m_max - m_min) + 1);
}

std::pair<qint64, qint64> DiceRollerNode::getRange() const
{
    return std::make_pair(m_min, m_max);
}
QString DiceRollerNode::toString(bool wl) const
{
    if(wl)
    {
        return QString("%1 [label=\"DiceRollerNode faces: %2\"]").arg(m_id).arg(getFaces());
    }
    else
    {
        return m_id;
    }
}
qint64 DiceRollerNode::getPriority() const
{
    qint64 priority= 4;
    //    if(nullptr!=m_nextNode)
    //    {
    //        priority = m_nextNode->getPriority();
    //    }
    return priority;
}
ExecutionNode* DiceRollerNode::getCopy() const
{
    DiceRollerNode* node= new DiceRollerNode(m_max, m_min);
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

Dice::ArithmeticOperator DiceRollerNode::getOperator() const
{
    return m_operator;
}

void DiceRollerNode::setOperator(const Dice::ArithmeticOperator& dieOperator)
{
    m_operator= dieOperator;
    m_diceResult->setOperator(dieOperator);
}

bool DiceRollerNode::getUnique() const
{
    return m_unique;
}

void DiceRollerNode::setUnique(bool unique)
{
    m_unique= unique;
}
