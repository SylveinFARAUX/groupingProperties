// pch.h : Il s'agit d'un fichier d'en-tête précompilé.
// Les fichiers listés ci-dessous sont compilés une seule fois, ce qui améliore les performances de génération des futures builds.
// Cela affecte également les performances d'IntelliSense, notamment la complétion du code et de nombreuses fonctionnalités de navigation du code.
// Toutefois, les fichiers listés ici sont TOUS recompilés si l'un d'entre eux est mis à jour entre les builds.
// N'ajoutez pas de fichiers fréquemment mis à jour ici, car cela annule les gains de performance.

#pragma once

#ifdef GROUPINGCORE_EXPORTS
#define GROUPINGCORE_API __declspec(dllexport)
#else
#define GROUPINGCORE_API __declspec(dllimport)
#endif

#include "framework.h"


namespace grouping
{
	class GROUPINGCORE_API GroupingTools
	{
	public:
		GroupingTools();
		static void run();
	};
}