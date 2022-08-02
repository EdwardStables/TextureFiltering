# Texture Filtering Toy

This is a toy project for implementing texture filtering algorithms. It uses the olcPixelGameEngine (and is licensed under the OLC-3 license).

I'll flesh out this readme with instructions as it gets more usable.

For now, the tool will pick up any png files placed in `media` and its subdirectories, allow you to pick the desired image from a list, and generate mipmaps of the images which you can move through. I haven't distributed the test images I've used, but you can find some decent examples [here](testimages.org).

## Issues
- Images are assumed to be 1200x1200 pixels
- Missing any kind of tool tips/ui for indicating controls and information
- FPS dramatically drops when in the menu
- Menu does not allow scrolling for reaching later images
- Menu does not allow searching


## Future Features
- Point, bilinear, and trilinear filtering
- Zooming in and panning
- Togglable dynamic LOD for matching mipmap/filtering to zoom
- Potentially 3d projection to demo:
    - Effect of varying LOD over a texture
    - Anisotropic filtering