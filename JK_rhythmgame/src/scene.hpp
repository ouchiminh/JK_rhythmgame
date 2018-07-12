#pragma once
namespace jk {
	enum SCENEFLAG{};

	class scene {
		virtual void finish() = 0;
	public:
		virtual bool free_resource() = 0;

		virtual SCENEFLAG render() = 0;
		virtual SCENEFLAG get_next_scene() = 0;
	};
}
