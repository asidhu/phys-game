#include "PhysGame\Source\PlatformDependent\GLFWGraphics.h"
#define GLEW_STATIC
#include "opengl\opengl.h"
#include "opengl\primitives.h"
#include "opengl\stb_image.h"
#include "PhysGame\Source\RenderList.h"
#define rad2deg(x) (x*57.2957795131f) 
primitives renderer;

static void error_callback(int error, const char* description)
{
	//fputs(description, stderr);
}
GLFWGraphics::GLFWGraphics(int w, int h){
	m_initialized = false;
	//setup error callback
	glfwSetErrorCallback(error_callback);
	//initiate GLFW
	if (!glfwInit()){
		return;
	}
	
	//create a window
	m_window = glfwCreateWindow(w, h, "GLFW", NULL, NULL);
	glfwMakeContextCurrent(m_window);
	GLenum err = GlewInit();
	if (err != GLEW_OK){
		return;
	}
	renderer.init();
	m_initialized = true;
	m_win_width = w;
	m_win_height = h;
	m_fb_width = m_fb_height = 0;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}
void GLFWGraphics::start(){
	float  ratio;
	int width, height;
	int nW, nH;
	float viewHeight = m_top - m_bottom;
	glfwGetFramebufferSize(m_window, &width, &height);
	glfwGetWindowSize(m_window, &m_win_width, &m_win_height);
	glfwSwapInterval(0);
	ratio = width / (float)height;
	if (width != m_fb_width || height != m_fb_height){
		glViewport(0, 0, width, height);
		m_fb_width = width;
		m_fb_height = height;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_left = m_centerX - viewHeight / 2 * ratio;
	m_right = m_centerX + viewHeight / 2 * ratio;
	renderer.setupViewport(m_left, m_right, m_top, m_bottom );
	

}

void GLFWGraphics::loadImage(unsigned int resID, char* filename){
	int x, y, comp;
	GLuint texID;
	unsigned char* data = stbi_load(filename, &x, &y, &comp, 0);
	glGenTextures(1, &texID); 
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, x, y, 0, (comp == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	stbi_image_free(data);
	m_resourceMap[resID] = texID;

}
void GLFWGraphics::freeImage(unsigned int id){
	assert(m_resourceMap.find(id) != m_resourceMap.end());
	GLuint texID = m_resourceMap[id];
	glDeleteTextures(1,&texID);
	m_resourceMap.erase(id);
}
void GLFWGraphics::close(){
	glfwSwapBuffers(m_window);
}
void GLFWGraphics::renderBatchTextureSquare(RenderList* list){
	GLint textures[MAX_BATCH];
	GLint texIDs[MAX_TEX];
	struct{
		GLfloat x, y;
		GLfloat rotation;
		GLfloat sx, sy;
		GLfloat tx, ty;
		GLint texID;
		GLfloat z;
	}instanceAttributes[MAX_BATCH];


	int num = 0, numTex = 0;
	while(list->batchTexSquare.size()){
		RenderItem* item = list->batchTexSquare.top();
		list->batchTexSquare.pop();
		instanceAttributes[num].x = item->x;
		instanceAttributes[num].y = item->y;
		instanceAttributes[num].z = item->zIndex;
		instanceAttributes[num].rotation = item->rot;
		instanceAttributes[num].sx = item->tex.w;
		instanceAttributes[num].sy = item->tex.h;
		instanceAttributes[num].tx = item->tex.s1;// 1.5f;
		instanceAttributes[num].ty = item->tex.s2;// 1.5f;
		GLint texid = m_resourceMap[item->tex.resID];
		if (numTex == 0 || texIDs[numTex - 1] != texid){
			texIDs[numTex] =  texid;
			instanceAttributes[num].texID = numTex;
			numTex++;

		}
		else{
			instanceAttributes[num].texID = numTex - 1;
		}
		num++;
		if (num == MAX_BATCH || numTex == MAX_TEX){
			renderer.batchSquareTexture(num, texIDs, numTex, instanceAttributes);
			num = numTex = 0;
		}
	}
	if (num != 0)
		renderer.batchSquareTexture(num, texIDs, numTex, instanceAttributes);

}
void GLFWGraphics::renderBatchCircle(bool hollow, std::list<RenderItem*> &list){
	struct{
		GLfloat x, y, radius, red, green, blue, alpha,z;
	}instanceAttributes[MAX_BATCH];
	int num = 0;
	for (std::list<RenderItem*>::iterator it = list.begin(); it != list.end(); ++it){
		RenderItem* item = *it;
		instanceAttributes[num].x = item->x;
		instanceAttributes[num].y = item->y;
		instanceAttributes[num].z = item->zIndex;
		instanceAttributes[num].radius = item->circle.radius;
		instanceAttributes[num].red = item->circle.r;
		instanceAttributes[num].green = item->circle.g;
		instanceAttributes[num].blue = item->circle.b;
		instanceAttributes[num].alpha = item->circle.a;
		num++;
		if (num == MAX_BATCH){
			if (hollow)
				renderer.batchDrawCircle(num, instanceAttributes);
			else
				renderer.batchCircle(num, instanceAttributes);
			num = 0;
		}
	}
	if (num != 0){
		if (hollow)
			renderer.batchDrawCircle(num, instanceAttributes);
		else
			renderer.batchCircle(num, instanceAttributes);
	}

}
void GLFWGraphics::renderBatchSquare(bool hollow, std::list<RenderItem*> &list){
	struct{
		GLfloat x, y, rot, sx, sy, red, green, blue, alpha,z;
	}instanceAttributes[MAX_BATCH];
	int num = 0;
	for (std::list<RenderItem*>::iterator it = list.begin(); it != list.end(); ++it){
		RenderItem* item = *it;
		instanceAttributes[num].x = item->x;
		instanceAttributes[num].y = item->y;
		instanceAttributes[num].z = item->zIndex;
		instanceAttributes[num].rot = item->rot;
		instanceAttributes[num].sx = item->square.w;
		instanceAttributes[num].sy = item->square.h;
		instanceAttributes[num].red = item->square.r;
		instanceAttributes[num].green = item->square.g;
		instanceAttributes[num].blue = item->square.b;
		instanceAttributes[num].alpha = item->square.a;
		num++;
		if (num == MAX_BATCH){
			if (hollow)
				renderer.batchDrawSquare(num, instanceAttributes);
			else
				renderer.batchSquare(num, instanceAttributes);
			num = 0;
		}
	}
	if (num != 0){
		if (hollow)
			renderer.batchDrawSquare(num, instanceAttributes);
		else
			renderer.batchSquare(num, instanceAttributes);
	}	

}
void GLFWGraphics::drawList(RenderList* list){
	renderBatchCircle(true,list->batchDrawCircle);
	renderBatchCircle(false,list->batchFillCircle);
	renderBatchSquare(true,list->batchDrawSquare);
	renderBatchSquare(false,list->batchFillSquare);
	renderBatchTextureSquare(list);
	for (std::list<RenderItem*>::iterator it = list->renderItems.begin(); it != list->renderItems.end(); ++it){
		RenderItem* item = *it;
		if (item->myType == solidsquare || item->myType == hollowsquare){
			renderer.setColor(item->square.r, item->square.g, item->square.b, item->square.a);
			if (item->myType == hollowsquare){
				renderer.setLineWidth(item->square.lw);
				renderer.drawRect(item->x, item->y, item->square.w, item->square.h, rad2deg(item->rot));
			}
			else
				renderer.fillRect(item->x, item->y, item->square.w, item->square.h, rad2deg(item->rot));
		}
		else if (item->myType == solidcircle || item->myType == hollowcircle){
			renderer.setColor(item->circle.r, item->circle.g, item->circle.b, item->circle.a);
			if (item->myType == hollowcircle){
				renderer.setLineWidth(item->circle.lw);
				renderer.drawCircle(item->x, item->y, item->circle.radius);
			}
			else
				renderer.fillCircle(item->x, item->y, item->circle.radius);
		}
		else if (item->myType == texture){
			GLuint texid = m_resourceMap[item->tex.resID];
			const float texcoords[8] = {
				item->tex.s1 + 100, item->tex.t1 + 100,
				item->tex.s1 + 100, item->tex.t2 + 100,
				item->tex.s2 + 100, item->tex.t2 + 100,
				item->tex.s2 + 100, item->tex.t1 + 100
			};
			renderer.drawTexture(texid, item->x, item->y,texcoords, item->tex.w, item->tex.h, rad2deg(item->rot));
			//renderer.drawTexture(texid, item->x, item->y, texcoords, item->tex.w, item->tex.h, rad2deg(item->rot));
		}
		else if (item->myType == stenciltexture){
			GLuint texid = m_resourceMap[item->tex.resID];
			float texcoords[8] = {
				item->tex.s1 + 100, item->tex.t1 + 100,
				item->tex.s1 + 100, item->tex.t2 + 100,
				item->tex.s2 + 100, item->tex.t2 + 100,
				item->tex.s2 + 100, item->tex.t1 + 100
			};
			draw_square mask, tex;
			tex.x = tex.y = 0;
			tex.w = tex.h = 100;
			mask.x = item->x;
			mask.y = item->y;
			mask.w = item->tex.w;
			mask.h = item->tex.h;
			renderer.drawStenciledTexture(texid, mask, tex, texcoords, rad2deg(item->rot));
		}
	}
}