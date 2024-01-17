// open_render_server.cpp: 定义应用程序的入口点。
//

#include "open_render_server.h"

bool exist(std::string& name) {
	if (FILE* file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

int main()
{
	httplib::Server server;
	server.Post("/initEngine", [](const httplib::Request& request, httplib::Response& response) {
		auto& body = request.body;
		rapidjson::Document document;
		document.Parse(body.c_str());
		auto engine_path = document["enginePath"].GetString();
		auto active_databases_json = document["activeDatabases"].GetArray();
		for (int i = 0; i < active_databases_json.Size(); i++) {
			auto database_id = active_databases_json[i].GetInt();
		}
		if (init_engine(engine_path) == 0) {
			response.status = 200;
		}
		else {
			response.status = 400;
		}
		return;
		});
	server.Get("/kill", [](const httplib::Request& request, httplib::Response& response) {
		stop();
		abort();
		});
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
		auto temp_path = std::filesystem::temp_directory_path().string() + "\openvocaloid\\";
		auto& body = request.body;
		rapidjson::Document document;
		document.Parse(body.c_str());
		std::string fname = document["filename"].GetString();
		if (exist(temp_path + fname + ".wav")) {
			response.status = 200;
			return;
		}
		std::ofstream file(temp_path + fname + ".json");
		file << document["content"].GetString();
		file.close();
		int res = render("D:\\93601\\Desktop\\sequence.json", "D:\\93601\\Desktop\\output.wav");
		if (res != 0) {
			response.status = 400;
			return;
		}
		response.status = 200;
		return;
		});
	server.listen("0.0.0.0", 6666);
	return 0;
}
