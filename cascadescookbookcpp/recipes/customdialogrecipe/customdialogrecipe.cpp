/* Copyright (c) 2012 Research In Motion Limited.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "customdialogrecipe.h"
#include "customdialogalarm.h"

#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/Container>
#include <bb/cascades/ImageView>
#include <bb/cascades/SequentialAnimation>
#include <bb/cascades/ScaleTransition>


using namespace bb::cascades;

CustomDialogRecipe::CustomDialogRecipe(Container * parent) :
        CustomControl(parent)
{
    Container *recipeContainer = new Container();
    recipeContainer->setLayout(new AbsoluteLayout());

    ImageView *flame = ImageView::create("asset:///images/customdialog/flame");
    flame->setLayoutProperties(AbsoluteLayoutProperties::create().x(180));

    // The pivot points are set to the middle bottom of the image
    // so that it can be scaled upwards in the animation below.
    flame->setPivot(445/2, 514/2);

    // The flame animation gradually scales the flame up in Y direction and
    // finally triggers the CustomDialog.
    mRisingFlame = SequentialAnimation::create(flame)
                    .add(ScaleTransition::create(flame).toY(1.2).duration(400))
                    .add(ScaleTransition::create(flame).toY(1.1).duration(400))
                    .add(ScaleTransition::create(flame).toY(1.4).duration(400))
                    .add(ScaleTransition::create(flame).toY(1.3).duration(400))
                    .add(ScaleTransition::create(flame).toY(1.6).duration(400))
                    .add(ScaleTransition::create(flame).toY(1.5).duration(400))
                    .add(ScaleTransition::create(flame).toY(1.9).duration(400))
                    .add(ScaleTransition::create(flame).toY(1.7).duration(400))
                    .add(ScaleTransition::create(flame).toY(2.0).duration(400))
                    .parent(this);

    connect(mRisingFlame, SIGNAL(ended()), this, SLOT(onHideAnimEnded()));

    ImageView *candle = ImageView::create("asset:///images/customdialog/background");

    // The CustomDialog is added as an attached object since it is visible in the
    // UI from the start. Since it is often the case that a dialog is used in many
    // different places in an application it is set up as a separate component, so that
    // it easily can be added to other Pages.
    mAlarmDialog = new CustomDialogAlarm(this);
    connect(mAlarmDialog, SIGNAL(visibleChanged(bool)), this, SLOT(onDialogVisible(bool)));

    recipeContainer->add(flame);
    recipeContainer->add(candle);

    setRoot(recipeContainer);

    mRisingFlame->play();
}

void CustomDialogRecipe::onHideAnimEnded()
{
    mAlarmDialog->setVisible(true);
}

void CustomDialogRecipe::onDialogVisible(bool visible)
{
    if(!visible) {
        mRisingFlame->play();
    }
}
