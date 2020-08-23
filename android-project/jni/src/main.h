enum { STILL, RIGHT, LEFT, UP, DOWN };

typedef struct {
 float x, y, z;
} vec3_t;


extern unsigned int clk;
extern vec3_t rot, trans;
extern vec3_t player_pos;
extern int round_dir;

extern float aspect_ratio;

extern int picture;
extern GLuint round_tex, over_tex;
extern GLuint round_left_tex, round_right_tex, round_up_tex, round_down_tex;
extern GLuint beg_tex;
extern GLuint back_tex, eye_tex;
extern GLuint pix_tex[14];

extern FILE *log_fp;
extern int width, height;
