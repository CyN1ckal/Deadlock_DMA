#include "pch.h"

#include "Aimpoints.h"

namespace Aimpoints
{
	AimList GetAimpoints(HeroId ID)
	{
		switch (ID)
		{
		case HeroId::Abrams:
			return { 7,15 };

		case HeroId::Bebop:
			return { 6,4 };

		case HeroId::Billy:
			return { 5,7 };

		case HeroId::Calico:
			return { 13,11 };

		case HeroId::Doorman:
			return { 20,18 };

		case HeroId::Drifter:
			return { 18, 8 };

		case HeroId::Dynamo:
			return { 23,26 };

		case HeroId::GreyTalon:
			return { 17,15 };

		case HeroId::Haze:
			return { 14,11 };

		case HeroId::Holliday:
			return { 13,11 };

		case HeroId::Infernus:
			return { 11,15 };

		case HeroId::Ivy:
			return { 13,10 };

		case HeroId::Kelvin:
			return { 12,9 };

		case HeroId::LadyGeist:
			return { 11,8 };

		case HeroId::Lash:
			return { 12, 10 };

		case HeroId::McGinnis:
			return { 38,36 };

		case HeroId::Mina:
			return { 16,14 };

		case HeroId::Mirage:
			return { 7,5 };

		case HeroId::MoKrill:
			return { 7,4 };

		case HeroId::Paige:
			return { 22,19 };

		case HeroId::Paradox:
			return { 8,6 };

		case HeroId::Pocket:
			return { 13,11 };

		case HeroId::Seven:
			return { 14,11 };

		case HeroId::Shiv:
			return { 13,11 };

		case HeroId::Sinclair:
			return { 7,5 };

		case HeroId::Victor:
			return  { 12,10 };

		case HeroId::Vindicta:
			return { 7,5 };

		case HeroId::Viscous:
			return { 7,5 };

		case HeroId::Vyper:
			return { 13,10 };

		case HeroId::Warden:
			return { 11,9 };

		case HeroId::Wraith:
			return { 7,5 };

		case HeroId::Yamato:
			return { 34,17 };
		}

		return{ 0,0 };
	}
}