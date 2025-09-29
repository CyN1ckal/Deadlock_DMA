#pragma once
#include "pch.h"

namespace HeroNames
{
	inline std::string Abrams = "Abrams";
	inline std::string Bebop = "Bebop";
	inline std::string Billy = "Billy";
	inline std::string Calico = "Calico";
	inline std::string Doorman = "Doorman";
	inline std::string Drifter = "Drifter";
	inline std::string Dynamo = "Dynamo";
	inline std::string GreyTalon = "Grey Talon";
	inline std::string Haze = "Haze";
	inline std::string Holliday = "Holliday";
	inline std::string Infernus = "Infernus";
	inline std::string Ivy = "Ivy";
	inline std::string Kelvin = "Kelvin";
	inline std::string LadyGeist = "Lady Geist";
	inline std::string Lash = "Lash";
	inline std::string McGinnis = "McGinnis";
	inline std::string Mina = "Mina";
	inline std::string Mirage = "Mirage";
	inline std::string MoKrill = "Mo+Krill";
	inline std::string Paige = "Paige";
	inline std::string Paradox = "Paradox";
	inline std::string Pocket = "Pocket";
	inline std::string Seven = "Seven";
	inline std::string Shiv = "Shiv";
	inline std::string Sinclair = "Sinclair";
	inline std::string Victor = "Victor";
	inline std::string Vindicta = "Vindicta";
	inline std::string Viscous = "Viscous";
	inline std::string Vyper = "Vyper";
	inline std::string Warden = "Warden";
	inline std::string Wraith = "Wraith";
	inline std::string Yamato = "Yamato";

	const inline std::unordered_map<int32_t, std::string_view> HeroNameMap = {
		{6,Abrams},
		{15,Bebop},
		{72,Billy},
		{16, Calico},
		{69, Doorman},
		{64,Drifter},
		{11, Dynamo},
		{17,GreyTalon},
		{13,Haze},
		{14,Holliday},
		{1,Infernus},
		{20,Ivy},
		{12,Kelvin},
		{4,LadyGeist},
		{30,Lash},
		{8,McGinnis},
		{63,Mina},
		{52,Mirage},
		{18,MoKrill},
		{67, Paige},
		{10, Paradox},
		{50, Pocket},
		{2, Seven},
		{19,Shiv},
		{60,Sinclair},
		{66,Victor},
		{3, Vindicta},
		{35, Viscous},
		{58, Vyper},
		{25, Warden},
		{7, Wraith},
		{27, Yamato}
	};
}

