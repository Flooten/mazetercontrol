/*
 * FILNAMN:       xmlcontrol.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-30
 *
 * BESKRIVNING:   XmlControl är en stödklass som förenklar
 *                in och utmatning i ett XML-formaterat dokument.
 *
 */

#ifndef XMLCONTROL_H
#define XMLCONTROL_H

#include <QDomDocument>
#include <QFile>
#include <QString>
#include <stdexcept>

class XmlException : public std::logic_error
{
public:
    XmlException(const QString& str) : std::logic_error(str.toStdString()) { }
};

class XmlControl
{
public:
    XmlControl(const QString& xmlfile);

    QString attributeValue(const QString& node_name, const QString& attribute);
    void setAttributeValue(const QString& node_name, const QString& attribute, const QString& value);

    QString text(const QString& parent_node_name);
    void setText(const QString& parent_node_name, const QString& text);

private:
    QString filename_;
    QDomDocument document_;

    const QString ATTRIBUTE_ERROR_ = "XML error: No such attribute.";

    void write();
    void read();

    QDomNode findNode(QDomNode node, const QString& node_name);
};

#endif
