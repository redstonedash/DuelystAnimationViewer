#include "Header.h"
int scroll_index = 0;
int unit_index = 0;
int animation_index = 0;
int animation_frame_number = 0;
int buttons_drawn_this_frame = 0;
int animation_count = 0;
Texture* active_spritesheet = nullptr;
std::vector <animation_frame_t>* animations = nullptr;
std::vector <std::string> animation_names;
std::vector<animation_frame_t>* animation_data = nullptr;

static void ChangeAnimation() {
	animation_data = &UnitAnimations[unit_names[unit_index]][animation_names[animation_index]];
	animation_count = animation_names.size();
	animation_frame_number = 0;
}

static void ChangeUnit() {
	animation_index = 0;
	active_spritesheet = &UnitSpriteSheets[unit_names[unit_index]];
	auto animationmap = UnitAnimations[unit_names[unit_index]];
	animation_names.clear();
	for (auto const& amap : animationmap)
		animation_names.push_back(amap.first);
	if (active_spritesheet->id == 0) {
		Texture tex = LoadTexture((paths[unit_index].substr(0, paths[unit_index].length()-3) + "png").c_str());
		SetTextureFilter(tex, TEXTURE_FILTER_POINT);
		UnitSpriteSheets[unit_names[unit_index]] = tex;
	}
	ChangeAnimation();
}


void drawUnitButton(int unitNumber) {
	Vector2 mp = GetMousePosition();
	if (IsMouseButtonPressed(0) && mp.x < 500 && mp.y > buttons_drawn_this_frame * 19 && mp.y < (buttons_drawn_this_frame + 1) * 19) {
		DrawText(unit_names[unitNumber].c_str(), 0, buttons_drawn_this_frame * 19, 18, SKYBLUE);
		unit_index = unitNumber;
		ChangeUnit();
	}
	else {
		DrawText(unit_names[unitNumber].c_str(), 0, buttons_drawn_this_frame * 19, 18, WHITE);
	}
}
void drawAnimationButton(int animationNumber) {
	Vector2 mp = GetMousePosition();
	if (IsMouseButtonPressed(0) && mp.x < 500 && mp.y > buttons_drawn_this_frame * 19 && mp.y < (buttons_drawn_this_frame + 1) * 19) {
		DrawText(animation_names[buttons_drawn_this_frame + scroll_index - unit_index - 1].c_str(), 25, buttons_drawn_this_frame * 19, 18, SKYBLUE);
		animation_index = buttons_drawn_this_frame + scroll_index - unit_index - 1;
		ChangeAnimation();
	}
	else {
		DrawText(animation_names[buttons_drawn_this_frame + scroll_index - unit_index - 1].c_str(), 25, buttons_drawn_this_frame * 19, 18, GRAY);
	}

}
int main(int argc, char* argv[]) {
	InitWindow(1024, 512, "Duelyst Sprite Viewer");
	SetTargetFPS(15);
	initAnimations(argv[0]);
	
	ChangeUnit();
	
	while (!WindowShouldClose()) {
		
		if (GetMouseWheelMove() != 0) {
			scroll_index -= GetMouseWheelMove();
			if (scroll_index < 0) {
				scroll_index = 0;
			}
			scroll_index %= unit_names.size();
		}
		buttons_drawn_this_frame = 0;
		for (; buttons_drawn_this_frame * 19 < GetScreenHeight() && buttons_drawn_this_frame + scroll_index < unit_names.size(); buttons_drawn_this_frame++) {
			if (buttons_drawn_this_frame + scroll_index > unit_index && buttons_drawn_this_frame + scroll_index <= unit_index + animation_count) {
				drawAnimationButton(buttons_drawn_this_frame + scroll_index - unit_index - 1);
			} 
			else if (buttons_drawn_this_frame + scroll_index <= unit_index + animation_count) {
				drawUnitButton(buttons_drawn_this_frame + scroll_index);
			}
			else {
				drawUnitButton(buttons_drawn_this_frame + scroll_index - animation_count);
			}
			
		}
		BeginDrawing();
		ClearBackground(BLACK);


		
		if (animation_data) {
			animation_frame_t animation_frame = animation_data->at(animation_frame_number);
			Vector2 sprite_position = { (float)animation_frame.position.x, (float)animation_frame.position.y };
			Vector2 sprite_size = { (float)animation_frame.size.x ,(float)animation_frame.size.y };
			DrawTexturePro(*active_spritesheet, { sprite_position.x, sprite_position.y, sprite_size.x, sprite_size.y }, { 512,0, 512,512 }, { 0,0 }, 0, WHITE);
			animation_frame_number++;
			animation_frame_number %= animation_data->size();
			
		}
		EndDrawing();
	}
}