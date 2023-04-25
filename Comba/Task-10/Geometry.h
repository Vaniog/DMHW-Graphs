#include "SFML/Graphics.hpp"
#include <cmath>
#include <bitset>
#include <cassert>

class Line {
public:
    Line(const sf::Vector2f& start, const sf::Vector2f& finish) :
        start_(start), finish_(finish) {
    };

    void Draw(sf::RenderWindow& window) {
        lines[0].position = start_;
        lines[1].position = finish_;
        window.draw(lines);
    }

    [[nodiscard]] sf::Vector2f GetIntersection(const Line& other) const {
        float k_1 = GetK();

        float k_2 = other.GetK();

        float b_1 = GetB();
        float b_2 = other.GetB();

        sf::Vector2f intersection;
        intersection.x = (b_2 - b_1) / (k_1 - k_2);

        intersection.y = intersection.x * k_1 + b_1;

        if (std::fabs(k_1) == 1e9f) {
            intersection.x = start_.x;
            intersection.y = intersection.x * k_2 + b_2;
        }
        if (std::fabs(k_2) == 1e9f) {
            intersection.x = other.start_.x;
            intersection.y = intersection.x * k_1 + b_1;
        }
        return intersection;
    }

    [[nodiscard]] bool Intersects(const Line& other) const {
        sf::Vector2f intersection = GetIntersection(other);
        if (OnLine(intersection) && other.OnLine(intersection)) {
            return true;
        }
        return false;
    }

    [[nodiscard]] bool OnLine(const sf::Vector2f point) const {
        float min_x = std::min(start_.x, finish_.x);
        float max_x = std::max(start_.x, finish_.x);
        float min_y = std::min(start_.y, finish_.y);
        float max_y = std::max(start_.y, finish_.y);
        return (point.x >= min_x && point.x <= max_x &&
            point.y >= min_y && point.y <= max_y);
    }

    [[nodiscard]] float GetK() const {
        float k = (finish_.y - start_.y) / (finish_.x - start_.x);
        k = std::min(k, 1e9f);
        k = std::max(k, -1e9f);
        return k;
    }

    [[nodiscard]] float GetB() const {
        return start_.y - start_.x * GetK();
    }

    sf::Vector2f start_;
    sf::Vector2f finish_;

private:
    sf::VertexArray lines = sf::VertexArray(sf::LinesStrip, 2);
};

class CatalanCircle {
    std::string data_;
    sf::Font font_;
public:
    void LoadData(const std::string& data) {
        data_ = data;
    }

    CatalanCircle(const sf::Vector2f& center, float radius, size_t points_amount,
                  const std::vector<std::pair<size_t, size_t>>& possible_lines) :
        center_(center), radius_(radius), points_amount_(points_amount), possible_lines_(possible_lines) {
        font_.loadFromFile("Font.ttf");
        float delta = 2.0f * (float) M_PI / (float) (points_amount_);
        float delta_s = (float) M_PI / 12;
        for (size_t i = 0; i < points_amount_; i++) {
            float alpha = delta_s - delta * (float) i;
            points.emplace_back(
                center_.x + cosf(alpha) * radius,
                center_.y + sinf(alpha) * radius
            );
        }
    }

    std::vector<Line> GetLinesFromId(const std::vector<bool>& id) {
        std::vector<Line> lines;
        for (int i = 0; i < possible_lines_.size(); i++) {
            if (id[i]) {
                lines.emplace_back(
                    points[possible_lines_[i].first],
                    points[possible_lines_[i].second]
                );
            }
        }
        return lines;
    }

    bool Catalan(const std::vector<bool>& id) {
        auto lines = GetLinesFromId(id);

        if (lines.size() != points_amount_ / 2) {
            return false;
        }

        for (uint32_t i = 0; i < lines.size(); i++) {
            for (uint32_t j = 0; j < lines.size(); j++) {
                if (i == j)
                    continue;
                if (lines[i].Intersects(lines[j])) {
                    return false;
                }
                if (lines[i].start_ == lines[j].start_ || lines[i].start_ == lines[j].finish_
                    || lines[i].finish_ == lines[j].start_ || lines[i].finish_ == lines[j].finish_) {
                    return false;
                }
            }
        }

        return true;
    }

    void Draw(sf::RenderWindow& window, const std::vector<bool>& id) {
        sf::CircleShape circle_shape(radius_ / 50, 30);
        circle_shape.setOrigin(circle_shape.getLocalBounds().width / 2,
                               circle_shape.getGlobalBounds().height / 2);

        sf::Text text;
        text.setFont(font_);
        text.setCharacterSize(23);
        text.setFillColor(sf::Color::White);

        for (int i = 0; i < points.size(); i++) {
            circle_shape.setPosition(points[i]);
            text.setPosition(points[i]);
            text.setString(sf::String(data_[i]));
            text.setOrigin(text.getLocalBounds().width / 2,
                           text.getLocalBounds().height / 2);

            //window.draw(circle_shape);
            window.draw(text);
        }

        auto lines = GetLinesFromId(id);
        for (auto& line : lines) {
            line.Draw(window);
        }
    }

private:
    sf::Vector2f center_;

    float radius_;

    std::vector<std::pair<size_t, size_t>> possible_lines_;

    size_t points_amount_;
    std::vector<sf::Vector2f> points;

};
