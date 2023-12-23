#pragma once

#include <curl/curl.h>

namespace Requests {
	size_t writeCallback(char* buffer, size_t size, size_t nmemb, std::string* data) {
		if (data == nullptr) {
			return 0;
		}

		data->append(buffer, size * nmemb);
		return size * nmemb;
	}

	class RequestHandler {
	private:
		const char* url;

	public:
		RequestHandler(const char* n_url) : url(n_url) {}

		std::string getData() {
			CURL* curl = curl_easy_init();
			std::string data;

			if (curl) {
				curl_easy_setopt(curl, CURLOPT_URL, url);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

				CURLcode res = curl_easy_perform(curl);

				if (res != CURLE_OK) {
					std::cerr << "- Error: " << curl_easy_strerror(res) << std::endl;
					data = "Error";
				}

				curl_easy_cleanup(curl);
			}

			return data;
		}
	};

	class WebhookHandler {
	private:
		const char* webhook_url;

	public:
		WebhookHandler(const char* url) : webhook_url(url) {}

		void sendMessage(std::string& message) {
			CURL* curl = curl_easy_init();

			if (curl) {
				curl_easy_setopt(curl, CURLOPT_URL, webhook_url);

				struct curl_slist* headers = NULL;
				headers = curl_slist_append(headers, "Content-Type: application/json");

				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());

				CURLcode res = curl_easy_perform(curl);

				if (res != CURLE_OK) {
					std::cerr << "- Error: " << curl_easy_strerror(res) << std::endl;
				}

				curl_easy_cleanup(curl);
				curl_slist_free_all(headers);
			}
		}
	};
}