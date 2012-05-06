#ifndef VMFLIB_H
#define VMFLIB_H

unsigned int vmf_id_counter = 1;

// ---
// BEGIN STRUCT DECLARATIONS

// defines a point in 3d space whose coordinates are integers
typedef struct vmf_point_3i_i
{
  int x;
  int y;
  int z;
} vmf_point_3i_i;

// defines a point in 3d space whose coordinates are doubles
typedef struct vmf_point_3d_d
{
  double x;
  double y;
  double z;
} vmf_point_3d_d;

// defines a point in 4d space whose coordinates are doubles
typedef struct vmf_point_4d_d
{
  double x;
  double y;
  double z;
  double w;
} vmf_point_4d_d;

// represents the versioninfo in a vmf file
typedef struct vmf_versioninfo
{
  int editorversion;
  int editorbuild;
  int mapversion;
  int formatversion;
  int prefab;
} vmf_versioninfo;

// represents a side in a vmf file
typedef struct vmf_side
{
  int id;
  vmf_point_3d_d planes[3];
  char* material;
  vmf_point_4d_d uaxis_c; double uaxis_s;
  vmf_point_4d_d vaxis_c; double vaxis_s;
  double rotation;
  int lightmapscale;
  int smoothing_groups;
} vmf_side;

// represents and editor in a vmf file
typedef struct vmf_editor
{
  vmf_point_3i_i color;
  int visgroupshown;
  int visgroupautoshown;
} vmf_editor;

// represents a solid in a vmf file
typedef struct vmf_solid
{
  int id;
  struct vmf_side sides[6];
  struct vmf_editor editor;
} vmf_solid;

// represents a world in a vmf file
typedef struct vmf_world
{
  int id;
  int mapversion;
  char* classname;
  char* skyname;
  int maxpropscreenwidth;
  char* detailvbsp;
  char* detailmaterial;
  vmf_solid *solids;
  unsigned long int _solid_size;
  unsigned long int _solid_capacity;
} vmf_world;

// represents a vmf file
typedef struct vmf_vmf
{
  struct vmf_versioninfo versioninfo;
  struct vmf_world world;
} vmf_vmf;

// END STRUCT DECLARATIONS
// ---

// ---
// BEGIN 'CONSTRUCTOR' DEFINITIONS

/**
 * Constructs a vmf.
 */
vmf_vmf vmf_build_vmf(vmf_versioninfo versioninfo, 
		      vmf_world world);

/**
 * Constructs a vmf_versioninfo, which holds the version information 
 * for a .vmf file.
 * Any argument with a value of -1 will be given a default value
 */
vmf_versioninfo vmf_build_versioninfo(int editorversion, 
				      int editorbuild, 
				      int mapversion, 
				      int formatversion, 
				      int prefab);

/**
 * Constructs a vmf_world, which holds the information for a world 
 * section in a .vmf file.
 * Any char* argument that is given a null pointer will take on a default 
 * value.
 */
vmf_world vmf_build_world(int mapversion,
			  char* classname,
			  char* skyname,
			  int maxpropscreenwidth,
			  char* detailvbsp,
			  char* detailmaterial);

vmf_solid vmf_build_solid(vmf_editor editor);

vmf_side vmf_build_side(vmf_point_3d_d plane_1, 
			vmf_point_3d_d plane_2, 
			vmf_point_3d_d plane_3, 
			char* material, 
			vmf_point_4d_d uaxis_c, double uaxis_s, 
			vmf_point_4d_d vaxis_c, double vaxis_s,
			double rotation, 
			int lightmapscale,
			int smoothing_groups);

vmf_editor vmf_build_editor(vmf_point_3i_i color,
			    int visgroupshown,
			    int visgroupautoshown);

// END 'CONSTRUCTOR' DEFINITIONS
// ---

// ---
// BEGIN FUNCTION DEFINITIONS

/**
 * id must be a number between 1 and 6 inclusive, or the function 
 * will do nothing.
 */
void vmf_solid_setSide(vmf_solid *solid, vmf_side side, int id);

void vmf_world_addSolid(vmf_world *world, vmf_solid solid);

// END FUNCTION DEFINITIONS
// ---

// ---
// BEGIN PRINTER FUNCTION DEFINITION

void vmf_fprint(FILE *f, vmf_vmf vmf);

// END PRINTER FUNCTION DEFINITION
// ---

#endif
