#pragma once
#include "pch.h"
#include "HeroEnum.hpp"

namespace HeroNames
{
	const inline std::string Abrams = "Abrams";
	const inline std::string Bebop = "Bebop";
	const inline std::string Billy = "Billy";
	const inline std::string Calico = "Calico";
	const inline std::string Doorman = "Doorman";
	const inline std::string Drifter = "Drifter";
	const inline std::string Dummy = "Dummy";
	const inline std::string Dynamo = "Dynamo";
	const inline std::string Graves = "Graves";
	const inline std::string GreyTalon = "Grey Talon";
	const inline std::string Haze = "Haze";
	const inline std::string Holliday = "Holliday";
	const inline std::string Infernus = "Infernus";
	const inline std::string Ivy = "Ivy";
	const inline std::string Kelvin = "Kelvin";
	const inline std::string LadyGeist = "Lady Geist";
	const inline std::string Lash = "Lash";
	const inline std::string McGinnis = "McGinnis";
	const inline std::string Mina = "Mina";
	const inline std::string Mirage = "Mirage";
	const inline std::string MoKrill = "Mo+Krill";
	const inline std::string Paige = "Paige";
	const inline std::string Paradox = "Paradox";
	const inline std::string Pocket = "Pocket";
	const inline std::string Rem = "Rem";
	const inline std::string Seven = "Seven";
	const inline std::string Shiv = "Shiv";
	const inline std::string Sinclair = "Sinclair";
	const inline std::string Victor = "Victor";
	const inline std::string Vindicta = "Vindicta";
	const inline std::string Viscous = "Viscous";
	const inline std::string Vyper = "Vyper";
	const inline std::string Warden = "Warden";
	const inline std::string Wraith = "Wraith";
	const inline std::string Yamato = "Yamato";

	const inline std::unordered_map<HeroId, std::string_view> HeroNameMap = {
		{HeroId::Abrams,Abrams},
		{HeroId::Bebop, Bebop},
		{HeroId::Billy, Billy},
		{HeroId::Calico, Calico},
		{HeroId::Doorman, Doorman},
		{HeroId::Drifter, Drifter},
		{HeroId::Dummy, Dummy},
		{HeroId::Dynamo, Dynamo},
		{HeroId::Graves, Graves},
		{HeroId::GreyTalon, GreyTalon},
		{HeroId::Haze, Haze},
		{HeroId::Holliday, Holliday},
		{HeroId::Infernus, Infernus},
		{HeroId::Ivy, Ivy},
		{HeroId::Kelvin, Kelvin},
		{HeroId::LadyGeist, LadyGeist},
		{HeroId::Lash, Lash},
		{HeroId::McGinnis, McGinnis},
		{HeroId::Mina, Mina},
		{HeroId::Mirage, Mirage},
		{HeroId::MoKrill, MoKrill},
		{HeroId::Paige, Paige},
		{HeroId::Paradox, Paradox},
		{HeroId::Pocket, Pocket},
		{HeroId::Rem, Rem},
		{HeroId::Seven, Seven},
		{HeroId::Shiv, Shiv},
		{HeroId::Sinclair, Sinclair},
		{HeroId::Victor, Victor},
		{HeroId::Vindicta, Vindicta},
		{HeroId::Viscous, Viscous},
		{HeroId::Vyper, Vyper},
		{HeroId::Warden, Warden},
		{HeroId::Wraith, Wraith},
		{HeroId::Yamato, Yamato}
	};
}

