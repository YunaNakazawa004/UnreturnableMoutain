xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 11;
 0.01037;-0.01939;-0.07237;,
 -1.97994;-0.00000;-0.00004;,
 0.01037;0.07862;0.02566;,
 -1.97994;-0.00000;-0.00004;,
 0.01037;-0.05526;0.06154;,
 -1.97994;-0.00000;-0.00004;,
 0.01037;-0.01939;-0.07237;,
 0.01037;-0.00001;-0.00004;,
 0.01037;-0.01939;-0.07237;,
 0.01037;0.07862;0.02566;,
 0.01037;-0.05526;0.06154;;
 
 6;
 3;0,1,2;,
 3;2,3,4;,
 3;4,5,6;,
 3;7,8,9;,
 3;7,9,10;,
 3;7,10,8;;
 
 MeshMaterialList {
  6;
  6;
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.482353;0.482353;0.482353;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.784000;0.784000;0.824000;1.000000;;
   16.000000;
   0.700000;0.700000;0.700000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;0.784000;0.392000;1.000000;;
   3.000000;
   0.100000;0.100000;0.100000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.200000;0.200000;0.200000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.196000;0.588000;1.000000;1.000000;;
   100.000000;
   0.600000;0.600000;0.600000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;0.588000;0.196000;1.000000;;
   5.000000;
   0.100000;0.100000;0.100000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  4;
  -0.018803;0.707056;-0.706908;,
  -0.022694;0.258825;0.965658;,
  1.000000;0.000000;0.000000;,
  -0.018809;-0.965768;-0.258723;;
  6;
  3;0,0,0;,
  3;1,1,1;,
  3;3,3,3;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;;
 }
 MeshTextureCoords {
  11;
  0.000000;1.000000;,
  0.166670;0.000000;,
  0.333330;1.000000;,
  0.333330;0.000000;,
  0.666670;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
