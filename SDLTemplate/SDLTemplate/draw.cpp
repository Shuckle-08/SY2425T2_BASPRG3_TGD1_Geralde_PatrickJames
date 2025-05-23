#include "draw.h"

void prepareScene(void)
{
	SDL_SetRenderDrawColor(app.renderer, 32, 32, 32, 255);
	SDL_RenderClear(app.renderer);
}

void presentScene(void)
{
	SDL_RenderPresent(app.renderer);
}

static SDL_Texture* getTexture(const char* name)
{
	Texture* t;

	for (t = app.textureHead.next; t != NULL; t = t->next)
	{
		if (strcmp(t->name, name) == 0)
		{
			return t->texture;
		}
	}

	return NULL;
}

static void addTextureToCache(const char* name, SDL_Texture* sdlTexture)
{
	Texture* texture = new Texture;

	memset(texture, 0, sizeof(Texture));
	app.textureTail->next = texture;
	app.textureTail = texture;

	STRNCPY(texture->name, name, MAX_NAME_LENGTH);
	texture->texture = sdlTexture;
}

SDL_Texture* loadTexture(const char* filename)
{
	SDL_Texture* texture;

	texture = getTexture(filename);

	if (texture == NULL)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
		texture = IMG_LoadTexture(app.renderer, filename);
		addTextureToCache(filename, texture);
	}

	return texture;
}

void blit(SDL_Texture* texture, int x, int y)
{
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y)
{
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	dest.w = src->w;
	dest.h = src->h;

	SDL_RenderCopy(app.renderer, texture, src, &dest);
}

void blitRotate(SDL_Texture* texture, int x, int y, double angle)
{
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopyEx(app.renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
}

void blitScale(SDL_Texture* texture, int x, int y, int* width, int* height, int scale)
{
	SDL_Rect dest;
	int tempW;
	int tempH;
	SDL_QueryTexture(texture, NULL, NULL, &tempW, &tempH);

	dest.x = x;
	dest.y = y;
	dest.w = tempW * scale;
	dest.h = tempH * scale;

	// Why? so it can automatically change the collisions for width and height, since width and height are in a separate universe
	*width = tempW * scale;
	*height = tempH * scale;

	SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}