#include "../headers/common.h"
#include <FreeImage.h>

unsigned char * Skybox::loadBMP(const char * imagepath, unsigned char *data, int * wd, int *ht){
	printf("Reading image %s\n", imagepath);

	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;

	FILE * file = fopen(imagepath,"rb");
	if (!file){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
		getchar();
		return NULL;
	}

	if ( fread(header, 1, 54, file)!=54 ){ 
		printf("Not a correct BMP file\n");
		fclose(file);
		return NULL;
	}

	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file\n");
		fclose(file);
		return NULL;
	}

	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    fclose(file); return NULL;}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    fclose(file); return NULL;}

	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	if (imageSize==0)    imageSize=width*height*3;
	if (dataPos==0)      dataPos=54; 

	data = (unsigned char *)malloc(imageSize * sizeof(unsigned char));
	fread(data,1,imageSize,file);
	fclose (file);

  *wd = width;
  *ht = height;
  return data;
}

FIBITMAP *loadImage(const char *filename){
    FIBITMAP *dib1 = NULL;
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filename);

    dib1 = FreeImage_Load(fif, filename, JPEG_DEFAULT);
    if (!dib1){
        return NULL;
    }
    return dib1;
}

unsigned char * loadTexture(FIBITMAP *dib1){
    if (dib1 == NULL)
        return NULL;
    int x, y;
    int height, width;

    RGBQUAD rgbquad;

    FREE_IMAGE_TYPE type;
    BITMAPINFOHEADER *header;
    type = FreeImage_GetImageType(dib1);
    height = FreeImage_GetHeight(dib1);
    width = FreeImage_GetWidth(dib1);
    header = FreeImage_GetInfoHeader(dib1);

    int scanLineWidth = ((3 * width) % 4 == 0) ? 3 * width : ((3 * width) / 4) * 4 + 4;
    unsigned char * texels = (GLubyte *)calloc(height * scanLineWidth, sizeof(GLubyte));   
    for (x = 0; x < width; x++){
        for (y = 0; y < height; y++){
            FreeImage_GetPixelColor(dib1, x, y, &rgbquad);
            texels[(y * scanLineWidth + 3 * x)] = ((GLubyte *)&rgbquad)[2];
            texels[(y * scanLineWidth + 3 * x) + 1] = ((GLubyte *)&rgbquad)[1];
            texels[(y * scanLineWidth + 3 * x) + 2] = ((GLubyte *)&rgbquad)[0];
        }
    }

    return texels;
}

unsigned int Skybox::loadCubemap(){
    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    int width, height;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        FIBITMAP *bmp = loadImage(faces[i].c_str());
        unsigned char * image = loadTexture(bmp);
        if (image)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, FreeImage_GetWidth(bmp), FreeImage_GetHeight(bmp), 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            free(image);
            free(bmp);
        }
        else {
            printf("Couldn't load all textures\n");
            free(bmp);
            free(image);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture_id;
}

Skybox::Skybox(const std::vector<std::string> &faces)
: faces(faces)
{
    this->shader = Shader("shaders/skybox_vtx_shader.glsl", "shaders/skybox_frag_shader.glsl");
    vertices = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    
    this->shader.use();
    this->texture = loadCubemap();
    this->texture_id  = shader.getUniformLocation("skybox");
    shader.putUniform1i(this->texture_id, 0);
    
    this->view_id = shader.getUniformLocation("view");
    this->proj_id = shader.getUniformLocation("projection");
    glUseProgram(0);
}

void Skybox::render_sky(){
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

std::vector<GLuint> Skybox::get_ids(){
    std::vector<GLuint> ids= {view_id, proj_id};
    return ids;
}