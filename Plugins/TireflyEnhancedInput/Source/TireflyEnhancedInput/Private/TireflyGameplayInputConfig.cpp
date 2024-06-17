// Copyright Tirefly. All Rights Reserved.


#include "TireflyGameplayInputConfig.h"

#include "InputMappingContext.h"
#include "TireflyInputAction.h"


void UTireflyGameplayInputConfig::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_CHECKED(UTireflyGameplayInputConfig, InputMappingContext))
	{
		if (InputMappingContext)
		{
			InputActionConfigs.Empty();

			for (const FEnhancedActionKeyMapping& Mapping : InputMappingContext->GetMappings())
			{
				if (UTireflyInputAction* InputAction = Cast<UTireflyInputAction>(Mapping.Action))
				{
					UTireflyInputActionConfig* ConfigIA = NewObject<UTireflyInputActionConfig>(this);
					ConfigIA->InputAction = InputAction;
					InputActionConfigs.Add(ConfigIA);
				}
			}
		}
	}
}
