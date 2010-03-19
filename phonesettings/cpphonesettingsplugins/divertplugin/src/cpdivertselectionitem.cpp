/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 *
 * Description:  
 *
 */
#include "cpdivertselectionitem.h"
#include "cpdivertselectioncustomitem.h"
#include <HbDataFormModel>
#include <HbAbstractItemView>
#include <cppluginlogging.h>
#include <qmetaobject.h>



CpDivertSelectionItem::CpDivertSelectionItem(QGraphicsItem *parent )
: HbDataFormViewItem(parent)
{
    DPRINT;
}

CpDivertSelectionItem::~CpDivertSelectionItem()
{
    DPRINT;
}

HbAbstractViewItem* CpDivertSelectionItem::createItem()
{
    DPRINT;
    return new CpDivertSelectionItem(*this);
}

bool CpDivertSelectionItem::canSetModelIndex(const QModelIndex &index) const
{
    DPRINT << "index:" << index;
    int type = index.data(HbDataFormModelItem::ItemTypeRole).toInt();
    return type == CpDivertSelectionItemId;
}

HbWidget *CpDivertSelectionItem::createCustomWidget()
{
    DPRINT << ": IN";

    int type = modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt();

    if (type == CpDivertSelectionItemId) {
            CpDivertSelectionCustomitem *retWidget =
                    new CpDivertSelectionCustomitem(this);

            DPRINT << ": OUT, retWidget";
            return retWidget;
    }
    DPRINT << ": OUT, 0";
    return 0;
}


void CpDivertSelectionItem::restore()
{
    HbDataFormViewItem::restore();

    if (dataItemContentWidget()) {
        HbDataFormModelItem::DataItemType itemType = static_cast<HbDataFormModelItem::DataItemType>(
            modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt());

        if (CpDivertSelectionItemId == itemType) {

            QModelIndex itemIndex = modelIndex();
            HbDataFormModel *model = static_cast<HbDataFormModel*>(itemView()->model());
            HbDataFormModelItem *modelItem = static_cast<HbDataFormModelItem*>(
                model->itemFromIndex(itemIndex));

            const QMetaObject *metaObj = dataItemContentWidget()->metaObject();
            int count = metaObj->propertyCount();
            for (int i = 0; i < count; i++) {
                QMetaProperty metaProperty = metaObj->property(i);
                if (metaProperty.isValid() && metaProperty.isWritable()) {
                    metaProperty.write(
                            dataItemContentWidget(),
                            modelItem->contentWidgetData(metaProperty.name()));
                }
            }
        }
    }

}
