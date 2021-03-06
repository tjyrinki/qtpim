/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtVersit module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qversitcontacthandler.h"

QT_BEGIN_NAMESPACE_VERSIT

/*!
  \internal
 */
QStringList QVersitContactHandlerFactory::keys() const
{
    return QStringList() << name();
}

/*!
  \class QVersitContactHandler
  \brief The QVersitContactHandler class is a union of the
  QVersitContactImporterPropertyHandlerV2 and QVersitContactExporterDetailHandlerV2 interfaces.
  \ingroup versit-extension
  \inmodule QtVersit
 */

/*!
  \fn QVersitContactHandler::~QVersitContactHandler()
  This frees any memory used by the QVersitContactHandler.
 */

/*!
  \class QVersitContactHandlerFactory
  \brief The QVersitContactHandlerFactory class provides the interface for Versit plugins.
  \ingroup versit-extension
  \inmodule QtVersit

  This class provides a simple interface for the creation of QVersitContactHandler instances.
  Implement this interface to write a Versit plugin.  For more details, see
  \l{Qt Versit Plugins}.
 */

/*!
   \fn static const QString QVersitContactHandlerFactory::ProfileSync()

   The constant string signifying a plugin that is relevant to import and export in a
   synchronization context.
   \sa profiles(),
   QVersitContactImporter::QVersitContactImporter(),
   QVersitContactExporter::QVersitContactExporter()
 */

/*!
   \fn static const QString QVersitContactHandlerFactory::ProfileBackup()

   The constant string signifying a plugin that will backup a QContact to vCard, so that exporting,
   then importing a contact will not lose any data.
   \sa profiles(),
   QVersitContactImporter::QVersitContactImporter(),
   QVersitContactExporter::QVersitContactExporter()
 */

/* TODO: make this a qdoc comment in 1.2
   \variable QVersitContactHandlerFactory::ProfilePreserve

   The constant string signifying a plugin that will preserve a vCard within a QContact, so that
   importing, then exporting a vCard will not lose any data.
   \sa profiles(),
   QVersitContactImporter::QVersitContactImporter(),
   QVersitContactExporter::QVersitContactExporter()
 */

/*!
  \fn QVersitContactHandlerFactory::~QVersitContactHandlerFactory()
  This frees any memory used by the QVersitContactHandlerFactory.
 */

/*!
  \fn QSet<QString> QVersitContactHandlerFactory::profiles() const
  This function can be overridden to allow a plugin to report which profiles it is to be active
  under.  If this (as in the default case) returns the empty set, it indicates that the plugin
  should be loaded under all profiles.  If it returns a non-empty set, it will only be loaded for
  those profiles that are specified by the importer/exporter class.
 */

/*!
  \fn QString QVersitContactHandlerFactory::name() const
  This function should return a unique string that identifies the handlers provided by this factory.
  Typically, this will be of the form "org.qt-project.Qt.SampleVersitContactHandler" with the
  appropriate domain and handler name substituted.
 */

/*!
  \fn int QVersitContactHandlerFactory::index() const
  This function should return an index that helps with determining the order in which to run the
  plugins.  Plugins are run in the following order:
  \list
  \li Positively-indexed, ascending
  \li Zero-indexed
  \li Negatively-indexed, ascending
  \endlist
  For example, plugins with an index of 1 are run first and plugins of index -1 are run last.
  If more than one plugin share an index, the order of execution between them is undefined.

  By default, this returns 0, which is recommended for plugins with no special ordering
  requirements.
 */

/*!
  \fn QVersitContactHandler* QVersitContactHandlerFactory::createHandler() const
  This function is called by the Versit importer or exporter class to create an instance of the
  handler provided by this factory.
 */

#include "moc_qversitcontacthandler.cpp"
QT_END_NAMESPACE_VERSIT
