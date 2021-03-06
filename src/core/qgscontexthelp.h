/***************************************************************************
                          qgscontexthelp.h
               Display context help for a dialog by invoking the
                                 QgsHelpViewer
                             -------------------
    begin                : 2005-06-19
    copyright            : (C) 2005 by Gary E.Sherman
    email                : sherman at mrcc.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSCONTEXTHELP_H
#define QGSCONTEXTHELP_H

#include <QObject>
#include <QHash>
#include <QProcess>

#include "qgis_core.h"
#include "qgis.h"

/** \ingroup core
 * Provides a context based help browser for a dialog.
 *
 * The help text is stored in SQLite and accessed by a context identifier
 * unique to each dialog. This is a singleton class which invokes the help
 * viewer using QProcess and ensures that only one viewer is open.
 * The viewer will be terminated if open when the main application quits.
 *
 */
class CORE_EXPORT QgsContextHelp : public QObject
{
    Q_OBJECT
  public:
    static void run( const QString &context );

  private slots:
    void processExited();
    void error( QProcess::ProcessError error );

  private:
    //! Constructor
    QgsContextHelp() SIP_FORCE;

    ~QgsContextHelp() SIP_FORCE;

    QProcess *start();
    void showContext( const QString &context );

    static QgsContextHelp *sContextHelp; // Singleton instance
    QProcess *mProcess = nullptr;

    static QHash<QString, QString> sContextHelpTexts;

    void init();
};

#endif //QGSCONTEXTHELP_H
