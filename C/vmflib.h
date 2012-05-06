#ifndef VMFLIB_H
#define VMFLIB_H

unsigned int vmf_id_counter = 1;

// ---
// BEGIN STRUCT DECLARATIONS

/**
 * Defines a point in 3D space, whose coordinates are represented by 
 * integer values.
 */
typedef struct vmf_point_3i_i
{
  int x;
  int y;
  int z;
} vmf_point_3i_i;

/**
 * Defines a point in 3D space, whose coordinates are represented by 
 * double-precision floating point values.
 */
typedef struct vmf_point_3d_d
{
  double x;
  double y;
  double z;
} vmf_point_3d_d;

/**
 * Defines a point in 4D space, whose coordinates are represented by 
 * double-precision floating point values.
 */
typedef struct vmf_point_4d_d
{
  double x;
  double y;
  double z;
  double w;
} vmf_point_4d_d;

/**
 * Represents the versioninfo section of a .vmf file
 * , which can be found at the beginning of the file.  It is used to 
 * describe the Hammer editor environment that is compatible with 
 * the .vmf file.
 */
typedef struct vmf_versioninfo
{
  /** Describes the version of Hammer that this file is compatible with */
  int editorversion;
  /** Describes the build of Hammer that this file is compatible with */
  int editorbuild;
  /** Describes the number of times this map has been saved */
  int mapversion;
  /**/
  int formatversion;
  /**/
  int prefab;
} vmf_versioninfo;

// represents a side in a vmf file
/**
 * Represents the side section of a .vmf file
 * .  This is used to hold information about the side of a solid (another 
 * vmf object).
 */
typedef struct vmf_side
{
  /** The ID number of the side.  Note that no two sides can have the same
      ID number. */
  int id;
  /** Holds the three points that determine the orientation of this side, 
      which is a plane. */
  vmf_point_3d_d plane_points[3];
  /** Describes the material that is mapped to this side as defined by Hammer, 
      e.g. "PLASTIC/PALSTICWALL001A" for plastic wall 1A. */
  char* material;
  /**/
  vmf_point_4d_d uaxis_c;
  /**/
  double uaxis_s;
  /**/
  vmf_point_4d_d vaxis_c;
  /**/
  double vaxis_s;
  /**/
  double rotation;
  /**/
  int lightmapscale;
  /**/
  int smoothing_groups;
} vmf_side;

/**
 * Represents a editor section in a .vmf file
 * , used in multiple places throughout the file.
 */
typedef struct vmf_editor
{
  /** The colour associated with this editor, represented by 3 points, 
      Red, Green, and Blue.*/
  vmf_point_3i_i color;
  /**/
  int visgroupshown;
  /**/
  int visgroupautoshown;
} vmf_editor;

// represents a solid in a vmf file
/**
 * Represents a solid section in a .vmf file
 * .  A solid is a physical object in a vmf, made up of sides and 
 * associated with an editor.
 */
typedef struct vmf_solid
{
  /** The ID number of this solid.  Should be unique from other IDs. */
  int id;
  /** Stores the 6 different side objects that make up the geometry 
      for this solid */
  struct vmf_side sides[6];
  /** Describes the editor associated with this solid */
  struct vmf_editor editor;
} vmf_solid;

/**
 * Represents a world in a vmf file
 * , which is made up of things such as solids.
 */
typedef struct vmf_world
{
  /** The ID number of this world, which should be unique to other IDs */
  int id;
  /**/
  int mapversion;
  /**/
  char* classname;
  /** Describes the skybox used with this world as defined by hammer, 
      e.g. "sky_day_01_01" for the first daytime skybox. */
  char* skyname;
  /**/
  int maxpropscreenwidth;
  /**/
  char* detailvbsp;
  /**/
  char* detailmaterial;
  /** An aggregation of all the solid objects that this world is made up of */
  vmf_solid *solids;
  unsigned long int _solid_size;
  unsigned long int _solid_capacity;
} vmf_world;

/**
 * Represents the overall .vmf file, containing everything that makes up 
 * a .vmf file.
 */
typedef struct vmf_vmf
{
  /** Describes the versioninfo associated with this vmf */
  struct vmf_versioninfo versioninfo;
  /** Describes the world associtaed with this vmf */
  struct vmf_world world;
} vmf_vmf;

// END STRUCT DECLARATIONS
// ---

// ---
// BEGIN 'CONSTRUCTOR' DECLARATIONS

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

// END 'CONSTRUCTOR' DECLARATIONS
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
