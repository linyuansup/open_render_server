// open_render_server.cpp: 定义应用程序的入口点。
//

#include "open_render_server.h"

std::default_random_engine e;
auto temp_path = std::filesystem::temp_directory_path().string() + "\openvocaloid\\";
std::string engine_path;

bool exist(std::string& name) {
	if (FILE* file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

int main(int argc, char* argv[])
{
	init_engine(argv[2]);
	e.seed(time(0));
	int res = mkdir(temp_path.c_str());
	httplib::Server server;
	server.Get("/database", [](const httplib::Request& request, httplib::Response& response) {
		rapidjson::Document databases;
		databases.SetArray();
		auto& allocator = databases.GetAllocator();
		auto db = get_databases();
		for (int i = 0; i < get_database_count(); i++) {
			rapidjson::Value database(rapidjson::kObjectType);
			database.AddMember("id", db[i].id, allocator);
			database.AddMember("name", rapidjson::Value(db[i].name, allocator).Move(), allocator);
			database.AddMember("langId", db[i].lang_id, allocator);
			databases.PushBack(database, allocator);
		}
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		databases.Accept(writer);
		std::string jsonString = buffer.GetString();
		response.status = 200;
		response.body = jsonString;
		return;
		});
	server.Post("/render", [](const httplib::Request& request, httplib::Response& response) {
		auto& body = request.body;
		rapidjson::Document document;
		document.Parse(body.c_str());
		std::string fname = std::to_string(e());
		std::ofstream file(temp_path + fname + ".json");
		file << document["content"].GetString();
		file.close();
		int res = render((temp_path + fname + ".json").c_str(), (temp_path + fname + ".wav").c_str());
		remove((temp_path + fname + ".json").c_str());
		if (res != 0) {
			response.status = 400;
			return;
		}
		std::ifstream result(temp_path + fname + ".wav", std::ios::binary);
		if (result) {
			result.seekg(0, std::ios::end);
			size_t length = result.tellg();
			result.seekg(0, std::ios::beg);
			std::vector<char> buffer(length);
			result.read(buffer.data(), length);
			remove((temp_path + fname + ".wav").c_str());
			response.set_content(buffer.data(), length, "application/octet-stream");
			response.set_header("Content-Disposition", "attachment; filename=result.wav");
		}
		else {
			response.status = 404;
		}
		response.status = 200;
		return;
		});
	server.listen("0.0.0.0", atoi(argv[1]));
	return 0;
}
