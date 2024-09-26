// file      :  prism_main_window.cpp
// birth     :  2/13/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "prism_main_window.h"
#include "windows/application_menu.h"
#include "windows/application_display.h"
#include "authentication/credential_manager.h"
#include "views/product_creation_navigator.h"
#include "views/inventory/stock_inventory_form.h"
#include "views/stock_entry_form.h"
#include "views/stock_creation_navigator.h"
#include <wposgui/views/content_lister_navigator.h>
#include <wposgui/views/catalog_browser.h>
#include <wposgui/widget/listing_form.h>

PrismMainWindow::PrismMainWindow(QWidget *parent)
    : BasicMainWindow{parent}
{
    setWindowTitle(QObject::tr("Prism SIGH [Système Intégré de Gestion Hospitalière]"));
}

void PrismMainWindow::installCredentialManager(){
    new CredentialManager(this);
}

ApplicationMenu* PrismMainWindow::createMenu(ApplicationDisplay* display){
    if( ! menu ){
        menu = new ApplicationMenu;

        //Material and Medicament
        QTreeWidgetItem *material_drug_menu = menu->topLevelItem(0);  //Material and Drug Menu
        QTreeWidgetItem *category_item = material_drug_menu->child(0); //Category
        category_item->setHidden(true);
        QTreeWidgetItem *medicament_item = material_drug_menu->child(1); //Medicament

        auto productCatalog = new CatalogBrowser;

        auto productListingNavigator = new ContentListerNavigator(new ListingForm, productCatalog);
        productCatalog->addTreeView(productListingNavigator);

        auto productCreationNavigator = new ProductCreationNavigator(productCatalog);
        productCreationNavigator->addPage();
        productCatalog->addFormView(productCreationNavigator);

        display->addPage(medicament_item, productCatalog);
        display->setCurrentWidget(productCatalog);

        QTreeWidgetItem *material_bed_ietm = material_drug_menu->child(2); //Beds
        material_bed_ietm->setHidden(true);
        // QTreeWidgetItem *material_material_item = material_drug_menu->child(3); //Medical Material
        QTreeWidgetItem *material_supplier_item = material_drug_menu->child(4); //Fournisseurs
        material_supplier_item->setHidden(true);

        QTreeWidgetItem *material_parameter_menu = material_drug_menu->child(5); //Parametrage
        material_parameter_menu->setHidden(true);
        // QTreeWidgetItem *uom_item = material_parameter_menu->child(0); //Unité de Mésure
        // QTreeWidgetItem *categorie_item = material_parameter_menu->child(1); //Parametre, Catégories
        // QTreeWidgetItem *product_parameters_item = material_parameter_menu->child(2); //Parametres, Produits

        //Hostpital Management
        QTreeWidgetItem *hospitam_nagement_menu = menu->topLevelItem(1); //Gestion Hospitalière
        // QTreeWidgetItem *patient_item = hospitam_nagement_menu->child(0); //Patients
        // QTreeWidgetItem *consultation_item = hospitam_nagement_menu->child(1); //Consultation
        // QTreeWidgetItem *appointment_item = hospitam_nagement_menu->child(2); //Rendez-Vous
        // QTreeWidgetItem *nursing_menu = hospitam_nagement_menu->child(3); //Soins Infirmier

        // QTreeWidgetItem *taking_parameters_item = nursing_menu->child(0); //Soins Infirmier , Prise de Paramètre
        // QTreeWidgetItem *ambulatory_care_item = nursing_menu->child(1); //Soins Infirmier , Soin Ambulatoire
        // QTreeWidgetItem *round_item = nursing_menu->child(2); //Soins Infirmier , Ronde
        // QTreeWidgetItem *hospitalization_item = hospitam_nagement_menu->child(4); //Hospitalisation
        // QTreeWidgetItem *ordonance_item = hospitam_nagement_menu->child(5); //Ordonance

        QTreeWidgetItem *lab_item = hospitam_nagement_menu->child(6); // Laboratoire
        lab_item->setHidden(true);
        QTreeWidgetItem *imaging_menu = hospitam_nagement_menu->child(7); //Imagerie Médicale
        imaging_menu->setHidden(true);
        // QTreeWidgetItem *health_professional_item = hospitam_nagement_menu->child(8); //Professionels de Santé


        //Stock and Inventory
        QTreeWidgetItem *stock_inventory_menu = menu->topLevelItem(2); //Stocks et Inventaires
        // QTreeWidgetItem *stock_state_item = stock_inventory_menu->child(0); //Etat du Stock
        QTreeWidgetItem *inventory_item = stock_inventory_menu->child(1); //Inventaire
        auto inventoryForm = new StockInventoryForm;
        display->addPage(inventory_item,inventoryForm);

        // QTreeWidgetItem *location_item = stock_inventory_menu->child(2); //Emplacement
        QTreeWidgetItem *shipping_menu = stock_inventory_menu->child(3); //Expédition
        shipping_menu->setHidden(true);
        // QTreeWidgetItem *internal_shipping_item = shipping_menu->child(0); //Expéditions Internes
        // QTreeWidgetItem *customer_shipping_item = shipping_menu->child(1); //Expéditions Clientes
        // QTreeWidgetItem *supplier_shipping_item = shipping_menu->child(2); //Expéditions Fournisseurs

        QTreeWidgetItem *stock_move_item = stock_inventory_menu->child(4); //Mouvement

        auto stockCatalog = new CatalogBrowser;

        auto inventoryListing = new ContentListerNavigator( new ListingForm, stockCatalog);
        stockCatalog->addTreeView(inventoryListing);

        auto inventoryCreation = new StockCreationNavigator(stockCatalog);
        inventoryCreation->addPage();
        stockCatalog->addFormView(inventoryCreation);

        display->addPage(stock_move_item, stockCatalog);

        // QTreeWidgetItem *lot_item = stock_inventory_menu->child(5); //Lots
        QTreeWidgetItem *suppliying_rules_item = stock_inventory_menu->child(6); //Règles d'Approvisionnement
        suppliying_rules_item->setHidden(true);
        // QTreeWidgetItem *stock_repport_item = stock_inventory_menu->child(7); //Rapports

        // Commands and Supply
        QTreeWidgetItem *command_supply_menu = menu->topLevelItem(3); // Commande et Livraison
        command_supply_menu->setHidden(true);
        // QTreeWidgetItem *command_item = command_supply_menu->child(0); //Commande
        // QTreeWidgetItem *supply_item = command_supply_menu->child(1); //Livraison

        //
        QTreeWidgetItem *party_menu = menu->topLevelItem(4); //Tiers
        party_menu->setHidden(true);
        // QTreeWidgetItem *insurance_item = party_menu->child(0); //Assurance
        // QTreeWidgetItem *pharmacy_item = party_menu->child(1); //Pharmacie
        // QTreeWidgetItem *institution_item = party_menu->child(2); //Institution
        // QTreeWidgetItem *employer_item = party_menu->child(3); //Employeur
        // QTreeWidgetItem *contact_mecanism_item = party_menu->child(4); //Moyens de Contact
        // QTreeWidgetItem *contact_item = contact_mecanism_item->child(0); //Contacts

        QTreeWidgetItem *hr_menu = menu->topLevelItem(5); //Ressources Humaines
        hr_menu->setHidden(true);
        // QTreeWidgetItem *department_item = hr_menu->child(0); //Départements
        // QTreeWidgetItem *employeee_item = hr_menu->child(1); //Personnel
        // QTreeWidgetItem *leave_item = hr_menu->child(2); //Congés
        // QTreeWidgetItem *schedule_item = hr_menu->child(3); //Planning
        // QTreeWidgetItem *attendence_item = hr_menu->child(4); //Présences
        // QTreeWidgetItem *enrolment_item = hr_menu->child(5); //Enrollement
        // QTreeWidgetItem *hr_repport_item = hr_menu->child(6); //Rapports

        QTreeWidgetItem *finance_menu = menu->topLevelItem(6); //Finance
        finance_menu->setHidden(true);
        // QTreeWidgetItem *invoice_menu = finance_menu->child(0); //Factures
        // QTreeWidgetItem *client_invoice_item = invoice_menu->child(0); //Factures Clients
        // QTreeWidgetItem *supplier_invoice_item = invoice_menu->child(1); //Factures Fournisseur
        // QTreeWidgetItem *input_item = finance_menu->child(1); //Entrées
        // QTreeWidgetItem *periodic_journal_item = input_item->child(0); //Journaux Périodiques
        // QTreeWidgetItem *finance_parameter_item = finance_menu->child(2); //Configuration

        QTreeWidgetItem *administration_menu = menu->topLevelItem(7); //Administration
        administration_menu->setHidden(true);
    }

    return menu;
}
