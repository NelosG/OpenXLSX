/*

   ____                               ____      ___ ____       ____  ____      ___
  6MMMMb                              `MM(      )M' `MM'      6MMMMb\`MM(      )M'
 8P    Y8                              `MM.     d'   MM      6M'    ` `MM.     d'
6M      Mb __ ____     ____  ___  __    `MM.   d'    MM      MM        `MM.   d'
MM      MM `M6MMMMb   6MMMMb `MM 6MMb    `MM. d'     MM      YM.        `MM. d'
MM      MM  MM'  `Mb 6M'  `Mb MMM9 `Mb    `MMd       MM       YMMMMb     `MMd
MM      MM  MM    MM MM    MM MM'   MM     dMM.      MM           `Mb     dMM.
MM      MM  MM    MM MMMMMMMM MM    MM    d'`MM.     MM            MM    d'`MM.
YM      M9  MM    MM MM       MM    MM   d'  `MM.    MM            MM   d'  `MM.
 8b    d8   MM.  ,M9 YM    d9 MM    MM  d'    `MM.   MM    / L    ,M9  d'    `MM.
  YMMMM9    MMYMMM9   YMMMM9 _MM_  _MM_M(_    _)MM_ _MMMMMMM MYMMMM9 _M(_    _)MM_
            MM
            MM
           _MM_

  Copyright (c) 2018, Kenneth Troldal Balslev

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
  - Neither the name of the author nor the
    names of any contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#ifndef OPENXLSX_IMPL_XLDOCUMENT_H
#define OPENXLSX_IMPL_XLDOCUMENT_H

// ===== Standard Library Includes ===== //
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>

// ===== OpenXLSX Includes ===== //
#include "XLAppProperties.hpp"
#include "XLCommand.hpp"
#include "XLCoreProperties.hpp"
#include "XLDefinitions.hpp"
#include "XLEnums.hpp"
#include "XLException.hpp"
#include "XLQuery.hpp"
#include "XLRelationships.hpp"
#include "XLWorkbook.hpp"
#include "XLXmlData.hpp"
#include "XLXmlParser.hpp"
#include "XLZipArchive.hpp"

namespace OpenXLSX
{
    /**
     * @brief This class encapsulates the concept of an excel file. It is different from the XLWorkbook, in that an
     * XLDocument holds an XLWorkbook together with its metadata, as well as methods for opening,
     * closing and saving the document.\n<b><em>The XLDocument is the entrypoint for clients
     * using the RapidXLSX library.</em></b>
     */
    class XLDocument final
    {
        //----- Friends
        friend class XLAbstractXMLFile;
        friend class XLWorkbook;
        friend class XLSheet;
        friend class XLXmlData;

        //---------- Public Member Functions
    public:
        /**
         * @brief Constructor. The default constructor with no arguments.
         */
        XLDocument();

        /**
         * @brief Constructor. An alternative constructor, taking the path to the .xlsx file as an argument.
         * @param docPath A std::string with the path to the .xlsx file.
         */
        explicit XLDocument(const std::string& docPath);

        /**
         * @brief Copy constructor
         * @param other The object to copy
         * @note Copy constructor explicitly deleted.
         */
        XLDocument(const XLDocument& other) = delete;

        /**
         * @brief
         * @param other
         */
        XLDocument(XLDocument&& other) noexcept = default;

        /**
         * @brief Destructor
         */
        virtual ~XLDocument();

        /**
         * @brief
         * @param other
         * @return
         */
        XLDocument& operator=(const XLDocument& other) = delete;

        /**
         * @brief
         * @param other
         * @return
         */
        XLDocument& operator=(XLDocument&& other) noexcept = default;

        /**
         * @brief Open the .xlsx file with the given path
         * @param fileName The path of the .xlsx file to open
         */
        void open(const std::string& fileName);

        /**
         * @brief Create a new .xlsx file with the given name.
         * @param fileName The path of the new .xlsx file.
         */
        void create(const std::string& fileName);

        /**
         * @brief Close the current document
         */
        void close();

        /**
         * @brief Save the current document using the current filename, overwriting the existing file.
         * @return true if successful; otherwise false.
         */
        bool save();

        /**
         * @brief Save the document with a new name. If a file exists with that name, it will be overwritten.
         * @param fileName The path of the file
         * @return true if successful; otherwise false.
         */
        bool saveAs(const std::string& fileName);

        /**
         * @brief Get the filename of the current document, e.g. "spreadsheet.xlsx".
         * @return A std::string with the filename.
         */
        const std::string& name() const;

        /**
         * @brief Get the full path of the current document, e.g. "drive/blah/spreadsheet.xlsx"
         * @return A std::string with the path.
         */
        const std::string& path() const;

        /**
         * @brief Get the underlying workbook object.
         * @return A pointer to the XLWorkbook object
         */
        XLWorkbook* workbook();

        /**
         * @brief Get the underlying workbook object, as a const object.
         * @return A const pointer to the XLWorkbook object.
         */
        const XLWorkbook* workbook() const;

        /**
         * @brief Get the requested document property.
         * @param prop The name of the property to get.
         * @return The property as a string
         */
        std::string property(XLProperty prop) const;

        /**
         * @brief Set a property
         * @param prop The property to set.
         * @param value The value of the property, as a string
         */
        void setProperty(XLProperty prop, const std::string& value);

        /**
         * @brief Delete the property from the document
         * @param propertyName The property to delete from the document
         */
        void deleteProperty(XLProperty theProperty);

        /**
         * @brief
         * @param command
         */
        void executeCommand(XLCommand command);

        /**
         * @brief
         * @param query
         * @return
         */
        std::string executeQuery(const XLQuery& query) const;

        //----------------------------------------------------------------------------------------------------------------------
        //           Protected Member Functions
        //----------------------------------------------------------------------------------------------------------------------

    protected:
        /**
         * @brief Get an XML file from the .xlsx archive.
         * @param path The relative path of the file.
         * @return A std::string with the content of the file
         */
        std::string extractXmlFromArchive(const std::string& path);

        /**
         * @brief
         * @param path
         * @return
         */
        XLXmlData* getXmlData(const std::string& path);

        /**
         * @brief
         * @param path
         * @return
         */
        const XLXmlData* getXmlData(const std::string& path) const;

        //----------------------------------------------------------------------------------------------------------------------
        //           Private Member Variables
        //----------------------------------------------------------------------------------------------------------------------

    private:
        std::string                  m_filePath {}; /**< The path to the original file*/
        mutable std::list<XLXmlData> m_data {};

        XLRelationships   m_docRelationships; /**< A pointer to the document relationships object*/
        XLRelationships   m_wbkRelationships; /**< A pointer to the document relationships object*/
        XLContentTypes    m_contentTypes;     /**< A pointer to the content types object*/
        XLAppProperties   m_appProperties;    /**< A pointer to the App properties object */
        XLCoreProperties  m_coreProperties;   /**< A pointer to the Core properties object*/
        XLWorkbook        m_workbook;         /**< A pointer to the workbook object */
        Zippy::ZipArchive m_archive;          /**<  */
    };

}    // namespace OpenXLSX

#endif    // OPENXLSX_IMPL_XLDOCUMENT_H
