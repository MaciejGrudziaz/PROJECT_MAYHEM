#pragma once
#include <time.h>
#include "Hitbox.h"
//#include <vector>
//#include <glm/glm.hpp>

//------------------------------------------------------------------------------------------------------
//-------------------------------------------DETEKCJA KOLIZJI-------------------------------------------
//--------------------klasa odpowiadaj�ca ze detekcj� kolizji obiekt�w w �wiecie gry--------------------
//------------------------------------------------------------------------------------------------------

const unsigned int RayCastLinesNum = 128;																					//liczba prostych u�ywanych przy algorytmie 'Ray Cast'

struct ModelCollision {
	bool status;																											//status kolizji: TRUE - kolizja zasz�a, FALSE - nie by�o kolizji
	//sk�adowe klasy okre�laj�ce detekcj� kolizji tylko z g��wnym hitboxem modelu/obiektu
	//u�ywane je�li dany model/obiekt posiada TYLKO g��wnego hitboxa
	//je�li zaszla kolizja z g��wnym hitboxem modelu ['Character']:						'mainColBox = TRUE', 'mainColBoxObjIdx = -1'
	//je�li zasz�a kolizja z g��wnym hitboxem obiektu ['Object' dla klasy 'Character']: 'mainColBox = TRUE', 'mainColBoxObjIdx = indeks_obiektu'
	bool mainColBox;																										//status kolizji z g��wnym hitboxem modelu/obiektu
	int mainColBoxObjIdx;																									//indeks obiektu dla kt�rego zasz�a kolizja z g��wnym hitboxem
	//wektor przechowuj�cy pary (objIdx, colBoxIdx)
	//'objIdx' - indeks obiektu z kt�ym zasz�a kolizja dla aktualnie sprawdzanego modelu
	//'colBoxIdx' - indeks dla bry�y kolizji dla obiektu opisanego poprzez 'indeks obiektu' z kt�r� zasz�a kolizja dla aktualnie sprawdzanego modelu
	std::vector<std::pair<unsigned int, unsigned int>> colIdx;

	ModelCollision() :status(false), mainColBox(false), mainColBoxObjIdx(-1) {}
};

//wektor przechowuj�cy pary (normal, linesCrossed) dla danej pary (objIdx,colBoxIdx)
//'normal' - wektor normalny do �ciany badanej bry�y kolizji ('colBoxIdx')
//'linesCrossed' - liczba prostych przechodz�cych przez �cian� opisan� przez normaln� 'normal' podczas testu 'Ray Cast' dla aktualnie badanej bry�y kolizji
typedef std::vector<std::pair<glm::vec3, unsigned int>> CollisionNormals;

class CollisionDetection {
	//-----------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------ALGORYTM GJK---------------------------------------------------------
	//------------------------------------------------funckje tworz�ce algorytm GJK------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------------------------
	static bool GJK(std::vector<glm::vec3>&model1, std::vector<glm::vec3>&model2);												//g��wna funkcja algorytmu GJK

	static glm::vec3 GJK_StartVector(std::vector<glm::vec3>&model1, std::vector<glm::vec3>&model2);									//wyliczanie pocz�tkowego wektora kierunkowego

	static glm::vec3 GJK_SupportFunction(std::vector<glm::vec3>&model1, std::vector<glm::vec3>&model2,glm::vec3 d);						//'Support function' dla GJK

	static bool GJK_SimplexContainsORIGIN(std::vector<glm::vec3>& Simplex);													//sprawdzanie czy Sympleks zawiera punkt (0,0,0)
	static void GJK_SimplexContainsORIGIN_CreateNormals(std::vector<glm::vec3>& Simplex, glm::vec3 n[]);							//funkcja pomocnicza - budowanie normalnych dla Sympleksa
	static void GJK_SimplexContainsORIGIN_CreateSurfaces(std::vector<glm::vec3>& Simplex, glm::vec3 n[], double Surface[][4]);	//funkcja pomocnicza - budowanie powierzchni dla Sympleksa

	static glm::vec3 GJK_GetDirection(std::vector<glm::vec3>&Simplex);															//funkcja wyznaczaj�ca wektor kierunkowy w kolejnych iteracjach algorytmu

	//Iteracja 1 [2 punkty w Sympleksie]
	static glm::vec3 GJK_GetDirection_Iteration_1(std::vector<glm::vec3>&Simplex);
	//Iteracja 2 [3 punkty w Sympleksie]
	static glm::vec3 GJK_GetDirection_Iteration_2(std::vector<glm::vec3>&Simplex);
	static glm::vec3 GJK_GetDirection_Iteration_2_AB(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AC, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_2_AC(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AC, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_2_CBA(glm::vec3 AB, glm::vec3 AC, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_2_ABC(glm::vec3 AB, glm::vec3 AC, glm::vec3 A0);
	//Iteracja 3 [4 punkty w Sympleksie]
	static glm::vec3 GJK_GetDirection_Iteration_3(std::vector<glm::vec3>&Simplex);
	static glm::vec3 GJK_GetDirection_Iteration_3_AB(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AC, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_3_AC(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AC, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_3_AD(std::vector<glm::vec3>&Simplex, glm::vec3 AC, glm::vec3 AD, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_3_ABC(std::vector<glm::vec3>&Simplex, glm::vec3 AC, glm::vec3 AB, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_3_ABD(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AD, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_3_ACD(std::vector<glm::vec3>&Simplex, glm::vec3 AD, glm::vec3 AC, glm::vec3 A0);

public:
	static bool CheckCollision(Hitbox* model1, Hitbox* model2);												//sprawdzanie kolizji pomi�dzy dwoma modelami
				
};