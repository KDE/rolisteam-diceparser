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
#ifndef SCALAROPERATORNODE_H
#define SCALAROPERATORNODE_H

#include <QChar>
#include <QMap>

#include "die.h"
#include "executionnode.h"
#include "result/scalarresult.h"

/**
 * @brief The ScalarOperatorNode class
 */
class ScalarOperatorNode : public ExecutionNode
{
public:
    /**
     * @brief The ArithmeticOperator enum
     */
    // enum ArithmeticOperator {PLUS,MINUS,DIVIDE,MULTIPLICATION};
    /**
     * @brief ScalarOperatorNode
     */
    ScalarOperatorNode();
    /**
     * @brief ~ScalarOperatorNode
     */
    virtual ~ScalarOperatorNode();
    /**
     * @brief run
     */
    virtual void run(ExecutionNode*) override;
    /**
     * @brief setInternalNode
     * @param node
     */
    void setInternalNode(ExecutionNode* node);
    /**
     * @brief toString
     * @param wl
     * @return
     */
    virtual QString toString(bool wl) const override;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const override;
    /**
     * @brief generateDotTree
     * @param s
     */
    void generateDotTree(QString& s) override;
    /**
     * @brief getErrorList
     * @return
     */
    virtual QMap<Dice::ERROR_CODE, QString> getExecutionErrorMap() override;
    /**
     * @brief getArithmeticOperator
     * @return
     */
    Dice::ArithmeticOperator getArithmeticOperator() const;
    /**
     * @brief setArithmeticOperator
     * @param arithmeticOperator
     */
    void setArithmeticOperator(const Dice::ArithmeticOperator& arithmeticOperator);

    /**
     * @brief getCopy
     * @return
     */
    virtual ExecutionNode* getCopy() const override;


    void setNextNode(ExecutionNode* node) override;

private:
    /**
     * @brief add
     * @return
     */
    static qint64 add(qreal, qreal);
    /**
     * @brief substract
     * @return
     */
    static qint64 substract(qreal, qreal);
    /**
     * @brief divide not static because of error management
     * @return
     */
    qreal divide(qreal, qreal);
    /**
     * @brief multiple
     * @return
     */
    static qint64 multiple(qreal, qreal);

    static qint64 pow(qreal a, qreal b);

private:
    ExecutionNode* m_internalNode;
    ScalarResult* m_scalarResult;
    Dice::ArithmeticOperator m_arithmeticOperator;
};

#endif // SCALAROPERATORNODE_H
