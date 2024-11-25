// Copyright  Mateusz Iwanek


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	
	WidgetControllerSet();

}
