#ifndef FILE_INFO_FUNCTIONS_H
#define FILE_INFO_FUNCTIONS_H

#endif // FILE_INFO_FUNCTIONS_H
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>


QString get_file_permission(QFileInfo &info) {
    QString result;
    if (info.permission(QFile::ReadOwner)) {
        result += "r";}
    if (!info.permission(QFile::ReadOwner)) {
        result += "-";}
    if(info.permission(QFile::WriteOwner)) {
        result += "w";}
    if(!info.permission(QFile::WriteOwner)) {
        result += "-";}
    if(info.permission(QFile::ExeOwner)) {
        result += "x";}
    if(!info.permission(QFile::ExeOwner)) {
        result += "-";}
    if(info.permission(QFile::ReadGroup)) {
        result += "r";}
    if(!info.permission(QFile::ReadGroup)) {
        result += "-";}
    if(info.permission(QFile::WriteGroup)) {
        result += "w";}
    if(!info.permission(QFile::WriteGroup)) {
        result += "-";}
    if(info.permission(QFile::ExeGroup)) {
        result += "x";}
    if(!info.permission(QFile::ExeGroup)) {
        result += "-";}
    if(info.permission(QFile::ReadOther)) {
        result += "r";}
    if(!info.permission(QFile::ReadOther)) {
        result += "-";}
    if(info.permission(QFile::WriteOther)) {
        result += "w";}
    if(!info.permission(QFile::WriteOther)) {
        result += "-";}
    if(info.permission(QFile::ExeOther)) {
        result += "x";}
    if(!info.permission(QFile::ExeOther)) {
        result += "-";
    }
    return result;
}

QString get_file_type(QFileInfo &info) {
    QString result;
    if (info.isDir()) {
        result = "Dir";
    } else if (info.isFile()) {
        result = "File";
    } else if (info.isSymLink()){
        result = "Symlink";
    } else {
        result = "Not recognised";
    }
    return result;
}
