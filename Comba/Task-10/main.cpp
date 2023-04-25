#include <SFML/Graphics.hpp>
#include "Geometry.h"
#include "chrono"
#include <iostream>

using ll = long long;

const float w_width = 2000;
const float w_height = 2000;

std::vector<std::vector<bool>> v;

int main() {
    sf::RenderWindow window(sf::VideoMode((ll)w_width,(ll)
    w_height), "SFML works!");

    std::vector<std::pair<size_t, size_t>> lines;

    std::map<std::pair<size_t, size_t>, size_t> lines_ind;

    std::string data = "CGUAAUUACGGCAUUAGCAU";

    std::vector<int> av;
    std::vector<int> uv;
    std::vector<int> gv;
    std::vector<int> cv;
    for (int i = 0; i < data.size(); i++)
        if (data[i] == 'A')
            av.emplace_back(i);
    for (int i = 0; i < data.size(); i++)
        if (data[i] == 'U')
            uv.emplace_back(i);
    for (int i = 0; i < data.size(); i++)
        if (data[i] == 'G')
            gv.emplace_back(i);
    for (int i = 0; i < data.size(); i++)
        if (data[i] == 'C')
            cv.emplace_back(i);

    for (auto& a : av) {
        for (auto& u : uv) {
            lines_ind.insert({{a, u}, lines.size()});
            lines.emplace_back(a, u);
        }
    }

    for (auto& g : gv) {
        for (auto& c : cv) {
            lines_ind.insert({{c, g}, lines.size()});
            lines.emplace_back(c, g);
        }
    }

    for (int i = 0; i < 720; i++) {
        std::vector<bool> new_v(52, false);

        for (int h = 0; h < 24; h++){
            for (int j = 0; j < uv.size(); j++) {
                new_v[lines_ind.find({av[j], uv[j]})->second] = true;
            }
            for (int j = 0; j < cv.size(); j++) {
                new_v[lines_ind.find({cv[j], gv[j]})->second] = true;
            }
            std::next_permutation(gv.begin(), gv.end());
            v.push_back(new_v);
            new_v.assign(52, false);
        }

        std::next_permutation(uv.begin(), uv.end());
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        ll cur_x = 1;
        ll cur_y = 1;

        std::vector<std::pair<std::vector<bool>, CatalanCircle>> circles;

        std::vector<bool> cur;
        cur.resize(lines.size());

        for (auto& id : v) {
            CatalanCircle catalan_circle({w_width / 2, w_height / 2}, w_height / 3,
                                         20,
                                         lines);
            if (catalan_circle.Catalan(id)) {
                circles.emplace_back(
                    id,
                    CatalanCircle{
                        {w_width / 6 * cur_x, w_height / 6 * cur_y},
                        w_height / 15,
                        20,
                        lines
                    });

                cur_x++;
                if (cur_x >= 6) {
                    cur_x = 1;
                    cur_y++;
                }
            }
        }

        for (auto& c : circles) {
            c.second.LoadData(data);
            c.second.Draw(window, c.first);
        }

        window.display();
        sf::sleep(sf::milliseconds(100));
    }

    return 0;
}
