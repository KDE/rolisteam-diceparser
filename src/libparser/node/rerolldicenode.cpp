#include <diceparser/parsingtoolbox.h>

#include "rerolldicenode.h"
#include "validatorlist.h"
#include <utility>

RerollDiceNode::RerollDiceNode(bool reroll, bool addingMode)
    : m_diceResult(new DiceResult()), m_validatorList(nullptr), m_reroll(reroll), m_adding(addingMode)
{
    m_result= m_diceResult;
}
RerollDiceNode::~RerollDiceNode()
{
    if(nullptr != m_validatorList)
    {
        delete m_validatorList;
        m_validatorList= nullptr;
    }
}
void RerollDiceNode::run(ExecutionNode* previous)
{
    if(isValid(!previous, Dice::ERROR_CODE::NO_PREVIOUS_ERROR, tr("No Previous node")))
        return;

    m_previousNode= previous;

    auto previousDiceResult= previous->getResult();
    if(isValid(!previousDiceResult, Dice::ERROR_CODE::NO_VALID_RESULT, tr("No Valid result")))
        return;

    DiceResult* previous_result= dynamic_cast<DiceResult*>(previousDiceResult);
    if(isValid(!previous_result, Dice::ERROR_CODE::DIE_RESULT_EXPECTED,
               tr(" The a operator expects dice result. Please check the documentation and fix your command.")))
        return;

    m_result->setPrevious(previous_result);

    for(auto& die : previous_result->getResultList())
    {
        Die* tmpdie= new Die(*die);
        m_diceResult->insertResult(tmpdie);
        die->displayed();
    }
    // m_diceResult->setResultList(list);

    QList<Die*>& list= m_diceResult->getResultList();
    QList<Die*> toRemove;

    for(auto& die : list)
    {
        bool finished= false;
        auto state
            = m_validatorList->isValidRangeSize(std::make_pair<qint64, qint64>(die->getBase(), die->getMaxValue()));
        if((Dice::CONDITION_STATE::ALWAYSTRUE == state && m_adding)
           || (!m_reroll && !m_adding && state == Dice::CONDITION_STATE::UNREACHABLE))
        {
            m_errors.insert(Dice::ERROR_CODE::ENDLESS_LOOP_ERROR,
                            QObject::tr("Condition (%1) cause an endless loop with this dice: %2")
                                .arg(toString(true))
                                .arg(QStringLiteral("d[%1,%2]")
                                         .arg(static_cast<int>(die->getBase()))
                                         .arg(static_cast<int>(die->getMaxValue()))));
            continue;
        }
        while(m_validatorList->hasValid(die, false) && !finished)
        {
            if(m_instruction != nullptr)
            {
                m_instruction->execute(this);
                auto lastNode= ParsingToolBox::getLeafNode(m_instruction);
                if(lastNode != nullptr)
                {
                    auto lastResult= dynamic_cast<DiceResult*>(lastNode->getResult());
                    if(lastResult != nullptr)
                    {
                        toRemove.append(die);
                        list.append(lastResult->getResultList());
                        lastResult->clear();
                    }
                }
            }
            else
            {
                die->roll(m_adding);
            }
            if(m_reroll)
            {
                finished= true;
            }
        }
    }

    for(auto die : toRemove)
    {
        list.removeOne(die);
    }
}
void RerollDiceNode::setValidatorList(ValidatorList* val)
{
    m_validatorList= val;
}
QString RerollDiceNode::toString(bool wl) const
{
    if(wl)
    {
        return QString("%1 [label=\"RerollDiceNode validatior: %2\"]").arg(m_id, m_validatorList->toString());
    }
    else
    {
        return m_id;
    }
    // return QString("RerollDiceNode [label=\"RerollDiceNode validatior:%1\"");
}
qint64 RerollDiceNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_nextNode)
    {
        priority= m_nextNode->getPriority();
    }

    return priority;
}
ExecutionNode* RerollDiceNode::getCopy() const
{
    RerollDiceNode* node= new RerollDiceNode(m_reroll, m_adding);
    node->setValidatorList(m_validatorList);
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

ExecutionNode* RerollDiceNode::getInstruction() const
{
    return m_instruction;
}

void RerollDiceNode::setInstruction(ExecutionNode* instruction)
{
    m_instruction= instruction;
}
