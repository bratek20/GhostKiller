#include "ViewFinder.h"
#include "Assets.h"
#include "Shader.h"

ViewFinder* makeViewFinder() {
    ViewFinder* vf = malloc(sizeof(ViewFinder));
    

    int dx[] = {-1,1,0,0};
    int dy[] = {0,0,-1,1};
    for (int i = 0; i < 4; i++) {
        vf->rects[i] = makeObject(SQUARE_SHAPE_2D, PLAIN_TEXTURE, VF_WIDTH, VF_HEIGHT, 0);
        setObjectPosition(vf->rects[i], dx[i] * VF_OFF, dy[i] * VF_OFF, 0);
        setObjectColor(vf->rects[i], 0, 0, 0, 1);
        if (i < 2) {
            setObjectRotationZ(vf->rects[i], 90);
        }
    }

    return vf;
}

void drawViewFinder(ViewFinder* vf){
    GLuint program = GLOBAL_PROGRAM;

    mat4x4 view;
    mat4x4 projection;
    mat4x4_identity(view); mat4x4_identity(projection);

    GLint viewLoc = glGetUniformLocation(program, "view");
    GLint projLoc = glGetUniformLocation(program, "projection");
    // Pass them to the shaders
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const GLfloat *)view);
    // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (const GLfloat *)projection);

    for (int i = 0; i < 4; i++)
        drawObject(vf->rects[i]);
}
