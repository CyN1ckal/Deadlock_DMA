#pragma once
#include "CBaseEntity.h"

class CTrooperEntity : public CBaseEntity
{
public:

public:
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
	{
		if (m_CurrentHealth < 1)
			SetInvalid();

		if (IsInvalid())
			return;

		CBaseEntity::PrepareRead_2(vmsh);
	}

	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
	{
		if (m_CurrentHealth < 1)
			SetInvalid();

		if (IsInvalid())
			return;

		CBaseEntity::QuickRead(vmsh, true);
	}
};