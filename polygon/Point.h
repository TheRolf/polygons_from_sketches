#pragma once

class Point
{
private:
	static int n_point;
public:
	float x, y;
	int serial_no;

	Point();
	Point(float, float);

	float distance(Point);
	float angle(Point, Point);

	friend bool operator< (const Point&, const Point&);
};
