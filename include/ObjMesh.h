/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OBJMESH_H
#define OBJMESH_H

#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <btBulletDynamicsCommon.h>
#include "Log.h"

class ObjMesh
{
public:
    ObjMesh() = default;
    ObjMesh(const std::string &filename, btScalar scale = 1.0f);
    void loadMesh(const std::string &filename, btScalar scale = 1.0f);
    std::unique_ptr<btTriangleMesh> getTriangleMesh() const;
    btVector3 *getPoints();
    std::size_t getPointsCount() const;
    void setPoints(btConvexHullShape &shape);

private:
    std::vector<btVector3> vertices;
    std::vector<std::vector<std::size_t>> polygons;

    struct Item {
        enum ItemType { SLASH, INT, FLOAT, KEYWORD, STRING, COMMENT, NEWLINE };

        Item(ItemType type) :
            type(type) {}

        Item(ItemType type, const std::string &text) :
            type(type), data((void *) new std::string(text)) {}

        Item(ItemType type, btScalar floatNumber) :
            type(type), data((void *) new btScalar(floatNumber)) {}

        Item(ItemType type, int intNumber) :
            type(type), data((void *) new int(intNumber)) {}

        Item(Item &&item) noexcept :
            type(item.type), data(item.data)
        {
            item.data = nullptr;
        }

        ~Item()
        {
            switch (type) {
            case INT:
                delete (int *) data;
                break;
            case FLOAT:
                delete (float *) data;
                break;
            case STRING:
            case KEYWORD:
            case COMMENT:
                delete (std::string *) data;
                break;
            default:
                break;
            }
        }

        std::string getString()
        {
            return *(std::string *) data;
        }

        const std::string &getString() const
        {
            return *(std::string *) data;
        }

        btScalar getFloat() const
        {
            return *(btScalar *) data;
        }

        int getInt() const
        {
            return *(int *) data;
        }

        static std::string typeToString(ItemType type)
        {
            switch (type) {
            case SLASH:
                return "slash";
            case INT:
                return "integer";
            case FLOAT:
                return "float";
            case STRING:
                return "string";
            case KEYWORD:
                return "keyword";
            case NEWLINE:
                return "new line or end of file";
            case COMMENT:
                return "comment";
            default:
                return "";
            }
        }

        ItemType type;
        void *data = nullptr;
    };
    static std::vector<Item> parse(const std::string &filename);
    void warning(Item::ItemType expected, Item::ItemType found);
};

#endif // OBJMESH_H
