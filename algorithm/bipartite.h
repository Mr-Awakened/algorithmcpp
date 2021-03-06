#pragma once
#include"graph.h"
#include"stack.h"
#include"graph_generator.h"
#include<vector>

namespace algorithmcpp {
	class Bipartite {
	private:
		bool is_bipartite_=true;
		std::vector<bool> color_;
		std::vector<bool> marked_;
		std::vector<size_t> edge_to_;
		Stack<size_t> cycle_;

	public:
		Bipartite() = delete;
		Bipartite(const Bipartite &) = default;
		Bipartite(Bipartite &&) noexcept = default;
		Bipartite &operator=(const Bipartite &) = default;
		Bipartite &operator=(Bipartite &&) noexcept = default;
		~Bipartite() = default;

		Bipartite(const Graph &G) :
			color_(std::vector<bool>(G.V())), marked_(std::vector<bool>(G.V())), edge_to_(std::vector<size_t>(G.V())) {
			for (size_t v = 0; v < G.V(); ++v) {
				if (!marked_[v]) {
					Dfs(G, v);
				}
			}
		}

	private:
		void Dfs(const Graph &G, size_t v) {
			marked_[v] = true;
			for (size_t w : G.Adj(v)) {
				if (!cycle_.IsEmpty()) return;
				if (!marked_[w]) {
					edge_to_[w] = v;
					color_[w] = !color_[v];
					Dfs(G, w);
				}
				else if (color_[w] == color_[v]) {
					is_bipartite_ = false;
					cycle_.Push(w);
					for (size_t x = v; x != w; x = edge_to_[x]) {
						cycle_.Push(x);
					}
					cycle_.Push(w);
				}
			}
		}

		void ValidateVertex(size_t v) const {
			if (v >= marked_.size())
				throw std::invalid_argument("vertex " + std::to_string(v) + " is not between 0 and " + std::to_string(marked_.size() - 1));
		}

	public:
		bool IsBipartite() const {
			return is_bipartite_;
		}

		bool Color(size_t v) const {
			ValidateVertex(v);
			if (!is_bipartite_) {
				throw std::logic_error("graph is not bipartite");
			}
			return color_[v];
		}

		Stack<size_t> OddCycle() const {
			return cycle_;
		}

		static void MainTest(int argc = 0, char *argv[] = nullptr) {
			size_t v1 = 10;
			size_t v2 = 10;
			size_t e = 20;
			size_t f = 15;

			Graph G = GraphGenerator::Bipartite(v1, v2, e);
			for (size_t i = 0; i < f; ++i) {
				size_t v = StdRandom::Uniform(v1 + v2);
				size_t w = StdRandom::Uniform(v1 + v2);
				G.AddEdge(v, w);
			}
			std::printf("%s\n", G.ToString().c_str());

			Bipartite b(G);
			if (b.IsBipartite()) {
				std::printf("Graph is bipartite\n");
				for (size_t v = 0; v < G.V(); ++v) {
					std::printf("%zd: %d\n", v, b.Color(v));
				}
			}
			else {
				std::printf("Graph has an odd - length cycle : ");
				for (size_t x : b.OddCycle()) {
					std::printf("%zd ", x);
				}
				std::printf("\n");
			}
		}
	};
}