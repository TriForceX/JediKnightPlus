jkmod_hitbox
{
	nopicmip
	{
		map gfx/misc/jkmod_hitbox.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}
jkmod_hitbox_nocull
{
	nopicmip
	cull none
	{
		map gfx/misc/jkmod_hitbox.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}
jkmod_chat_icon
{
	nopicmip
	nomipmaps
    {
        map gfx/mp/jkmod_chat_icon
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
    }
}
gfx/2d/net
{
    nopicmip
    nomipmaps
    {
        map gfx/2d/net
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
    }
}