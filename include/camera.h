#ifndef __CAMERA_H__
#define __CAMERA_H__

enum CAMERA_DIRECTION
{
    FORWARD,
    BACK,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    ROTATE_DOWN,
    ROTATE_UP,
    ROTATE_LEFT,
    ROTATE_RIGHT,
};

void Camera_Init    ( void );
void Camera_Deinit  ( void );
void Camera_Draw    ( void );
void Camera_Update  ( float DeltaTime );
void Camera_Reset   ( void );

void Camera_StartMove(CAMERA_DIRECTION dir);
void Camera_StopMove(CAMERA_DIRECTION dir);

#endif
