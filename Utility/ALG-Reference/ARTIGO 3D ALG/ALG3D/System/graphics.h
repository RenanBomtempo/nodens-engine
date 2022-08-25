#ifndef GRAPHICS_H
#define GRAPHICS_H

void setColor( int colorNumber );
void setLineWidth( float size );
void drawString( const char* string, void* font, float x, float y, float z);
void drawPoint( float x, float y, float z, float size );
void drawLine( float x0, float y0, float z0, float x1, float y1, float z1);
void drawSphere( float x, float y, float z, float radius );
void drawCube( float x0, float y0, float z0, float x1, float y1, float z1);
void drawSolidCube( float x0, float y0, float z0, float x1, float y1, float z1);
void enableLighting();
void disableLighting();

#endif
