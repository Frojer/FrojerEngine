#include "ModelLoader.h"
#include <FEUtility.h>

using namespace std;

#define BUFFER_SIZE 512
#define TCSCMP_SAME(buf, str) _tcsnicmp((buf), (str), wcslen((str)) + 1) == 0

ModelLoader::ModelLoader()
{

}

ModelLoader::~ModelLoader()
{

}

void ModelLoader::ASEFileParce(const TCHAR* fileName)
{
	TCHAR buf[BUFFER_SIZE];
	wifstream f(fileName);

	tstring filePath, tempString;
	GetFilePath(fileName, filePath);

	int a, b, c, i;
	float x, y, z, w;

	if (f.fail())
		return;

	while (!f.eof())
	{
		f >> buf;

		#pragma region IsCommand?
		if (buf[0] != '*')
		{
			// If isn't Command, line clear;
			f.getline(buf, BUFFER_SIZE);
			continue;
		}
		#pragma endregion

		#pragma region SCENE
		if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE")))
		{
			f >> buf;
			
			while (true)
			{
				f >> buf;

				// If is '}', it's mean work done. so end while
				if (buf[0] == '}') break;

				#pragma region IsCommand?
				if (buf[0] != '*')
				{
					// If isn't Command, line clear;
					f.getline(buf, BUFFER_SIZE);
					continue;
				}
				#pragma endregion
			}
		}
		#pragma endregion

		#pragma region MATERIAL_LIST
		else if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_LIST")))
		{
			f >> buf;
			
			while (true)
			{
				f >> buf;

				// If is '}', it's mean work done. so end while
				if (buf[0] == '}') break;

				#pragma region IsCommand?
				if (buf[0] != '*')
				{
					// If isn't Command, line clear;
					f.getline(buf, BUFFER_SIZE);
					continue;
				}
				#pragma endregion

				#pragma region MATERIAL_COUNT
				if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_COUNT")))
				{
					f >> i;
					pMtrls.resize(i);
				}
				#pragma endregion

				#pragma region MATERIAL
				if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL")))
				{
					f >> i;

					pMtrls[i] = new FEMaterial(FEShader::Find(FE_TEXT("Standard")));

					f >> buf;

					while (true)
					{
						f >> buf;

						// If is '}', it's mean work done. so end while
						if (buf[0] == '}') break;

						#pragma region IsCommand?
						if (buf[0] != '*')
						{
							// If isn't Command, line clear;
							f.getline(buf, BUFFER_SIZE);
							continue;
						}
						#pragma endregion

						// MATERIAL_NAME 
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_NAME")))
						{
							TCHAR name[BUFFER_SIZE];
							f.getline(buf, BUFFER_SIZE);
							a = 0;
							while (buf[a] != L'"')	a++;
							int s = ++a;
							while (buf[a] != L'"')	a++;
							int e = a;
							wcsncpy_s(name, buf + s, e - s);
							pMtrls[i]->m_Name = name;
						}


						// MATERIAL_CLASS
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_CLASS")))
						{
							f >> buf;

							if (TCSCMP_SAME(buf, FE_TEXT("\"Standard\"")))
							{
								// MATERIAL_AMBIENT
								f >> buf >> pMtrls[i]->m_ambient.x >> pMtrls[i]->m_ambient.y >> pMtrls[i]->m_ambient.z;
								// MATERIAL_DIFFUSE
								f >> buf >> pMtrls[i]->m_diffuse.x >> pMtrls[i]->m_diffuse.y >> pMtrls[i]->m_diffuse.z;
								// MATERIAL_SPECULAR
								f >> buf >> pMtrls[i]->m_specular.x >> pMtrls[i]->m_specular.y >> pMtrls[i]->m_specular.z;
								// MATERIAL_SHINE
								f >> buf >> pMtrls[i]->m_fPower;
								pMtrls[i]->m_fPower *= 100;
								// MATERIAL_SHINESTRENGTH9
								f >> buf >> x;
								//pMtrls[i]->m_Specular = pMtrls[i]->m_Specular * x;
								// MATERIAL_TRANSPARENCY
								f >> buf >> x;
								pMtrls[i]->m_diffuse.w = 1 - x;
								// MATERIAL_WIRESIZE
								f >> buf >> x;
								// MATERIAL_SHADING
								f >> buf >> buf;
								// MATERIAL_XP_FALLOFF
								f >> buf >> x;
								// MATERIAL_SELFILLUM
								f >> buf >> x;
								// MATERIAL_FALLOFF
								f >> buf >> buf;
								// MATERIAL_XP_TYPE
								f >> buf >> buf;
							}
						}

						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_DIFFUSE")))
						{
							f >> buf;

							f >> buf;

							// MAP_NAME
							TCHAR name[BUFFER_SIZE];
							f.getline(buf, BUFFER_SIZE);
							a = 0;
							while (buf[a] != L'"')	a++;
							int s = ++a;
							while (buf[a] != L'"')	a++;
							int e = a;
							_tcsncpy_s(name, buf + s, e - s);

							// 만약 이 텍스쳐가 이미 존재한다면 스킵

							// 없다면 진행
							FETexture* tex = new FETexture(buf);
							tex->m_Name = name;
							pMtrls[i]->m_pTexture[0] = tex;

							// MAP_CLASS
							f >> buf >> buf;
							// MAP_SUBNO
							f >> buf >> a;
							// MAP_AMOUNT
							f >> buf >> tex->m_fAmount;
							// BITMAP
							f >> buf;
							f.getline(buf, BUFFER_SIZE);
							GetFileName(buf, tex->m_Name);
							tex->Create((filePath + tex->m_Name).c_str());
							// MAP_TYPE
							f >> buf >> buf;
							// UVW_U_OFFSET
							f >> buf >> tex->m_vOT.x;
							// UVW_V_OFFSET
							f >> buf >> tex->m_vOT.y;
							// UVW_U_TILING
							f >> buf >> tex->m_vOT.z;
							// UVW_V_TILING
							f >> buf >> tex->m_vOT.w;
							// UVW_ANGLE
							f >> buf >> tex->m_fAngle;
							// UVW_BLUR
							f >> buf >> x;
							// UVW_BLUR_OFFSET
							f >> buf >> x;
							// UVW_NOUSE_AMT
							f >> buf >> x;
							// UVW_NOISE_SIZE
							f >> buf >> x;
							// UVW_NOISE_LEVEL
							f >> buf >> x;
							// UVW_NOISE_PHASE
							f >> buf >> x;
							// BITMAP_FILTER
							f >> buf >> buf;
						}
					}
				}
				#pragma endregion
			}
		}
		#pragma endregion

		#pragma region GEOMOBJECT
		else if (TCSCMP_SAME(buf + 1, FE_TEXT("GEOMOBJECT")))
		{
			CreateModel();

			f >> buf;

			while (true)
			{
				f >> buf;

				// If is '}', it's mean work done. so end while
				if (buf[0] == '}') break;

				#pragma region IsCommand?
				if (buf[0] != '*')
				{
					// If isn't Command, line clear;
					f.getline(buf, BUFFER_SIZE);
					continue;
				}
				#pragma endregion
				
				#pragma region NODE_NAME
				if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_NAME")))
				{
					TCHAR name[BUFFER_SIZE];
					i = 0;

					f.getline(buf, BUFFER_SIZE);

					while (buf[i] != L'"')	i++;
					int s = ++i;
					while (buf[i] != L'"')	i++;
					int e = i;

					_tcsncpy_s(name, buf + s, e - s);

					LoadModelName(name);
				}
				#pragma endregion
				
				#pragma region NODE_PARENT
				if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_PARENT")))
				{
					TCHAR name[BUFFER_SIZE];
					i = 0;

					f.getline(buf, BUFFER_SIZE);

					while (buf[i] != L'"')	i++;
					int s = ++i;
					while (buf[i] != L'"')	i++;
					int e = i;

					_tcsncpy_s(name, buf + s, e - s);

					SetParent(name);
				}
				#pragma endregion

				#pragma region NODE_TM
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_TM")))
				{
					f >> buf;

					while (true)
					{
						f >> buf;

						if (buf[0] == '}') break;

						#pragma region IsCommand?
						if (buf[0] != '*')
						{
							// If isn't Command, line clear;
							f.getline(buf, BUFFER_SIZE);
							continue;
						}
						#pragma endregion

						if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_POS")))
						{
							f >> x >> y >> z;
							LoadPosition(x, y, z);
						}

						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROTAXIS")))
						{
							f >> x >> y >> z;
							//
						}

						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROTANGLE")))
						{
							f >> x;
							//
						}

						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_SCALE")))
						{
							f >> x >> y >> z;
							LoadScale(x, y, z);
						}

						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_SCALEAXIS")))
						{
							f >> x >> y >> z;
							// 
						}

						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_SCALEAXISANG")))
						{
							f >> x;
							//
						}
					}
				}
				#pragma endregion

				#pragma region MESH
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH")))
				{
					f >> buf;

					while (true)
					{
						f >> buf;

						// If is '}', it's mean work done. so end while
						if (buf[0] == '}') break;
						
						#pragma region IsCommand?
						if (buf[0] != '*')
						{
							// If isn't Command, line clear;
							f.getline(buf, BUFFER_SIZE);
							continue;
						}
						#pragma endregion

						// Vertex Position
						#pragma region MESH_NUMVERTEX
						if (TCSCMP_SAME(buf + 1, FE_TEXT("TIMEVALUE")))
						{
							f >> i;
						}
						#pragma endregion
						
						// Vertex Position
						#pragma region MESH_NUMVERTEX
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMVERTEX")))
						{
							f >> i;
							
							vPos.resize(i);
						}
						#pragma endregion

						#pragma region MESH_VERTEX_LIST
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_VERTEX_LIST")))
						{
							f >> buf;

							while (true)
							{
								f >> buf;
								if (buf[0] == '}') break;

								f >> i >> x >> y >> z;

								vPos[i].x = x;
								vPos[i].y = z;
								vPos[i].z = y;
							}
						}
						#pragma endregion
						
						#pragma region MESH_NUMFACES
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMFACES")))
						{
							f >> i;

							iPos.resize(i);
						}
						#pragma endregion
						
						#pragma region MESH_FACE_LIST
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_FACE_LIST")))
						{
							f >> buf;

							while (true)
							{
								f >> buf;
								if (buf[0] == '}') break;

								TCHAR count[16];
								f >> count >> buf >> a >> buf >> b >> buf >> c;

								i = 0;
								while (count[i] != L'\0') i++;
								count[--i] = L'\0';
								i = _wtoi(count);

								iPos[i].a = a;
								iPos[i].b = b;
								iPos[i].c = c;
								f.getline(buf, BUFFER_SIZE);
							}
						}
						#pragma endregion
						
						// Texture
						#pragma region MESH_NUMTVERTEX
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMTVERTEX")))
						{
							f >> i;

							vTex.resize(i);
						}
						#pragma endregion
						
						#pragma region MESH_TVERTLIST
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_TVERTLIST")))
						{
							f >> buf;

							while (true)
							{
								f >> buf;
								if (buf[0] == '}') break;

								f >> i >> x >> y >> z;

								vTex[i].x = x;
								vTex[i].y = y;
							}
						}
						#pragma endregion
						
						#pragma region MESH_NUMTVFACES
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMTVFACES")))
						{
							f >> i;

							iTex.resize(i);
						}
						#pragma endregion
						
						#pragma region MESH_TFACELIST
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_TFACELIST")))
						{
							f >> buf;

							while (true)
							{
								f >> buf;
								if (buf[0] == '}') break;

								f >> i >> a >> b >> c;

								iTex[i].a = a;
								iTex[i].b = b;
								iTex[i].c = c;
							}
						}
						#pragma endregion
						
						// Vertex Color
						#pragma region MESH_NUMCVERTEX
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMCVERTEX")))
						{
							f >> i;
							vColor.resize(i);
						}
						#pragma endregion
						
						#pragma region MESH_CVERTLIST
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_CVERTLIST")))
						{
							f >> buf;

							while (true)
							{
								f >> buf;
								if (buf[0] == '}') break;

								f >> i >> x >> y >> z;

								vColor[i].x = x;
								vColor[i].y = y;
								vColor[i].z = z;
								vColor[i].w = 1;
							}
						}
						#pragma endregion
						
						#pragma region MESH_NUMCVFACES
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMCVFACES")))
						{
							f >> i;
							iColor.resize(i);
						}
						#pragma endregion
						
						#pragma region MESH_CFACELIST
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_CFACELIST")))
						{
							f >> buf;

							while (true)
							{
								f >> buf;
								if (buf[0] == '}') break;

								f >> i >> a >> b >> c;

								iColor[i].a = a;
								iColor[i].b = b;
								iColor[i].c = c;
							}
						}
						#pragma endregion

						// Normal
						#pragma region MESH_NORMALS
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NORMALS")))
						{
							int normalCount = 0;

							f >> buf;

							vVertexNormal.resize(iPos.size() * 3);
							vFaceNormal.resize(iPos.size());

							while (true)
							{
								f >> buf;

								if (buf[0] == '}') break;
								
								#pragma region IsCommand?
								if (buf[0] != '*')
								{
									// If isn't Command, line clear;
									f.getline(buf, BUFFER_SIZE);
									continue;
								}
								#pragma endregion

								if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_FACENORMAL")))
								{
									f >> i >> x >> y >> z;

									vFaceNormal[i].x = x;
									vFaceNormal[i].y = z;
									vFaceNormal[i].z = y;
								}

								else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_VERTEXNORMAL")))
								{
									f >> i >> x >> y >> z;

									vVertexNormal[normalCount].x = x;
									vVertexNormal[normalCount].y = z;
									vVertexNormal[normalCount].z = y;

									normalCount++;
								}
							}
						}
						#pragma endregion
					}
				}
				#pragma endregion

				/*else if (TCSCMP_SAME(buf + 1, L"PROP_MOTIONBLUR"))
				{

				}

				else if (TCSCMP_SAME(buf + 1, L"PROP_CASTSHADOW"))
				{

				}

				else if (TCSCMP_SAME(buf + 1, L"PROP_RECVSHADOW"))
				{

				}*/
				
				#pragma region MATERIAL_REF
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_REF")))
				{
					f >> i;
					SetMaterial(i);
				}
				#pragma endregion
			}

			FinishLoadModel();
		}
		#pragma endregion
	}

	f.close();
}