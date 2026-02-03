#include "pch.h"

#include "Aimpoints.h"

namespace Aimpoints
{
	// Returns optimal aim bone indices for each hero
	// First value is head bone, second value is chest/torso bone
	AimList GetAimpoints(HeroId ID)
	{
		switch (ID)
		{
		case HeroId::Abrams:
			return { 7,6 };

		case HeroId::Bebop:
			return { 6,4 };

		case HeroId::Billy:
			return { 7,6 };

		case HeroId::Calico:
			return { 13,12 };

		case HeroId::Doorman:
			return { 20,6 };

		case HeroId::Drifter:
			return { 18, 8 };

		case HeroId::Dynamo:
			return { 23,22 };

		case HeroId::Graves:
			return { 13, 12 };

		case HeroId::Dummy:
			return { 12,10 };

		case HeroId::GreyTalon:
			return { 17,5 };

		case HeroId::Haze:
			return { 14,11 };

		case HeroId::Holliday:
			return { 13,11 };

		case HeroId::Infernus:
			return { 28,27 };

		case HeroId::Ivy:
			return { 13,10 };

		case HeroId::Kelvin:
			return { 12,10 };

		case HeroId::LadyGeist:
			return { 11,9 };

		case HeroId::Lash:
			return { 12, 10 };

		case HeroId::McGinnis:
			return { 38,37 };

		case HeroId::Mina:
			return { 16,14 };

		case HeroId::Mirage:
			return { 7,6 };

		case HeroId::MoKrill:
			return { 7,4 };

		case HeroId::Paige:
			return { 22,18 };

		case HeroId::Paradox:
			return { 8,7 };

		case HeroId::Pocket:
			return { 13,4 };

		case HeroId::Seven:
			return { 14,11 };

		case HeroId::Silver:
			return { 9,8 };

		case HeroId::Apollo:
			return { 20,19 };

		case HeroId::Celeste:
			return { 11,10 };

		case HeroId::Venator:
			return { 18,17 };

		case HeroId::Rem:
			return { 10,7 };

		case HeroId::Shiv:
			return { 13,11 };

		case HeroId::Sinclair:
			return { 7,6 };

		case HeroId::Victor:
			return  { 12,10 };

		case HeroId::Vindicta:
			return { 7,5 };

		case HeroId::Viscous:
			return { 7,5 };

		case HeroId::Vyper:
			return { 13,11 };

		case HeroId::Warden:
			return { 11,9 };

		case HeroId::Wraith:
			return { 7,6 };

		case HeroId::Yamato:
			return { 34,17 };
		}

		return{ 0,0 };
	}
}