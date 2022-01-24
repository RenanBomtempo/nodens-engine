#pragma once

#include <string>


namespace Moxxi {
	class Shader {
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Use() const;
		void Unbind() const;
	private:
		uint32_t m_RendererID;

	};
}