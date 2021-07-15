#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "ImguiUtil.h"

void imguiSetup() {
	// Starting the new ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void drawWinScreen() {
	ImGui::SetWindowFontScale(5.f);
	ImGui::Text("You Win!");
	ImGui::Text("Press R to restart");
}

void drawScoreBoard(const int score) {
	// Putting the text-containing window in the top-left of the screen.
	ImGui::SetNextWindowPos(ImVec2(5, 5));

	// Setting flags
	ImGuiWindowFlags textWindowFlags =
		ImGuiWindowFlags_NoMove |				// text "window" should not move
		ImGuiWindowFlags_NoResize |				// should not resize
		ImGuiWindowFlags_NoCollapse |			// should not collapse
		ImGuiWindowFlags_NoSavedSettings |		// don't want saved settings mucking things up
		ImGuiWindowFlags_AlwaysAutoResize |		// window should auto-resize to fit the text
		ImGuiWindowFlags_NoBackground |			// window should be transparent; only the text should be visible
		ImGuiWindowFlags_NoDecoration |			// no decoration; only the text should be visible
		ImGuiWindowFlags_NoTitleBar;			// no title; only the text should be visible

	// Begin a new window with these flags. (bool *)0 is the "default" value for its argument.
	ImGui::Begin("scoreText", (bool*)0, textWindowFlags);

	// Scale up text a little, and set its value
	ImGui::SetWindowFontScale(1.5f);
	ImGui::Text("Score: %d", score); // Second parameter gets passed into "%d"}
}

void imguiRender() {
	// End the window.
	ImGui::End();
	ImGui::Render();	// Render the ImGui window
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Some middleware thing
}

void imguiShutdown() {
	// ImGui cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
