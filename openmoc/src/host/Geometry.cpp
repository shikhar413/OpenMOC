#include "Geometry.h"


/**
 * @brief Constructor initializes an empty geometry.
 */
Geometry::Geometry() {

    /* Initializing the corners to be infinite  */
    _x_min = std::numeric_limits<double>::max();
    _y_min = std::numeric_limits<double>::max();
    _x_max = -std::numeric_limits<double>::max();;
    _y_max = -std::numeric_limits<double>::max();;

    _max_seg_length = 0;
    _min_seg_length = std::numeric_limits<double>::infinity();

    /* Default boundary conditions are reflective */
    _top_bc = 1;
    _bottom_bc = 1;
    _left_bc = 1;
    _right_bc = 1;

    _num_FSRs = 0;
    _num_groups = 0;
}


/**
 * @brief Destructor clears all references to materials, surfaces, cells,
 *        universes and lattices.
 */
Geometry::~Geometry() {

    _materials.clear();
    _surfaces.clear();
    _cells.clear();
    _universes.clear();
    _lattices.clear();

    /* Free FSR to cells and materials maps if they were initialized */
    if (_num_FSRs != 0) {
        delete [] _FSRs_to_cells;
        delete [] _FSRs_to_materials;
    }
}


/**
 * @brief Returns the total height (y extent) of the geometry in cm.
 * @return the total height of the geometry (cm)
 */
double Geometry::getHeight() {
    return (_y_max - _y_min);
}


/**
 * @brief Returns the total width (x extent) of the geometry in cm.
 * @return the total width of the geometry (cm)
 */
double Geometry::getWidth() {
    return (_x_max - _x_min);
}


/**
 * @brief Return the minimum x-coordinate contained by the geometry.
 * @return the minimum x-coordinate (cm)
 */
double Geometry::getXMin() {
    return _x_min;
}


/**
 * @brief Return the maximum x-coordinate contained by the geometry.
 * @return the maximum x-coordinate (cm)
 */
double Geometry::getXMax() {
    return _x_max;
}


/**
 * @brief Return the minimum y-coordinate contained by the geometry.
 * @return the minimum y-coordinate (cm)
 */
double Geometry::getYMin() {
    return _y_min;
}


/**
 * @brief Return the maximum y-coordinate contained by the geometry.
 * @return the maximum y-coordinate (cm)
 */
double Geometry::getYMax() {
    return _y_max;
}



/**
 * @brief Returns the boundary condition for the top surface of the geometry.
 * @details The boundary conditions are vacuum (false) and reflective (false).
 * @return the boundary conditions for the top of the geometry
 */
bool Geometry::getBCTop() {
    return _top_bc;
}


/**
 * @brief Returns the boundary condition for the bottom surface of the geometry.
 * @details The boundary conditions are vacuum (false) and reflective (false).
 * @return the boundary conditions for the bottom of the geometry
 */
bool Geometry::getBCBottom() {
    return _bottom_bc;
}


/**
 * @brief Returns the boundary condition for the left surface of the geometry.
 * @details The boundary conditions are vacuum (false) and reflective (false).
 * @return the boundary conditions for the left surface of the geometry
 */
bool Geometry::getBCLeft() {
    return _left_bc;
}


/**
 * @brief Returns the boundary condition for the right surface of the geometry.
 * @details The boundary conditions are vacuum (false) and reflective (false).
 * @return the boundary conditions for the right surface of the geometry
 */
bool Geometry::getBCRight() {
    return _right_bc;
}


/**
 * @brief Returns the number of flat source regions in the geometry.
 * @return number of flat source regions
 */
int Geometry::getNumFSRs() {
    return _num_FSRs;
}


/**
 * @brief Returns the number of energy groups for each material's nuclear data.
 * @return the number of energy groups
 */
int Geometry::getNumEnergyGroups() {
    if (getNumMaterials() == 0)
        log_printf(ERROR, "Unable to return the number of energy groups from "
		   "the geometry since it does not contain any materials");

    return _num_groups;
}


/**
 * @brief Returns the number of materials in the geometry.
 * @return the number of materials
 */
int Geometry::getNumMaterials() {
    return _materials.size();
}


/**
 * @brief Return an array indexed by flat source region IDs which contain 
 *        the corresponding cell IDs.
 * @return an array map of flat source region to cell IDs
 */
int* Geometry::getFSRtoCellMap() {
    if (_num_FSRs == 0)
        log_printf(ERROR, "Unable to return the FSR to cell map array since "
                 "the geometry has not initialized FSRs.");

    return _FSRs_to_cells;
}


/**
 * @brief Return an array indexed by flat source region IDs which contain
 *        the corresponding material IDs.
 * @return an array map of flat source region to material IDs
 */
int* Geometry::getFSRtoMaterialMap() {
    return _FSRs_to_materials;
}


/**
 * @brief Return the maximum segment length computed during segmentation (cm)
 * @return max segment length (cm)
 */
double Geometry::getMaxSegmentLength() {
    return _max_seg_length;
}


/**
 * @brief Return the minimum segment length computed during segmentation (cm)
 * @return min segment length (cm)
 */
double Geometry::getMinSegmentLength() {
    return _min_seg_length;
}


/**
 * Return the map container of pointers to materials stored with their
 * ids as keys
 * @return a map of materials in the geometry
 */
std::map<short int, Material*> Geometry::getMaterials() {
        return _materials;
}


/**
 * @brief Return a pointer to a material in the geometry.
 * @param id the user-specified material ID
 * @return a pointer to the material object
 */
Material* Geometry::getMaterial(short int id) {

    Material* material = NULL;
  
    try {
        material = _materials.at(id);
    }
    catch (std::exception & e) {
        log_printf(ERROR, "Attempted to retrieve material with id = %d which"
                   " does not exist. Backtrace:\n%s", id, e.what());
    }

    return material;
}


/**
 * @brief Return a pointer to a surface from the geometry.
 * @param id the user-specified surface ID
 * @return a pointer to the surface object
 */
Surface* Geometry::getSurface(short int id) {

    Surface* surface = NULL;

    try {
        surface = _surfaces.at(id);
    }
    catch (std::exception & e) {
        log_printf(ERROR, "Attempted to retrieve surface with id = %d which "
                   "has not been declared. Backtrace:\n%s", id, e.what());
    }

    return surface;
}


/**
 * @brief Return a pointer to a cell from the geometry.
 * @param id the user-specified cell's ID
 * @return a pointer to the cell object
 */
Cell* Geometry::getCell(short int id) {

    Cell* cell = NULL;

    try {
        cell = _cells.at(id);
    }
    catch (std::exception & e) {
        log_printf(ERROR, "Attempted to retrieve cell with id = %d which has "
                   "not been declared. Backtrace:\n%s", id, e.what());
    }

    return cell;
}


/**
 * @brief Return a pointer to a universe from the geometry.
 * @param id the user-specified universe ID
 * @return a pointer to the universe object
 */
Universe* Geometry::getUniverse(short int id) {

    Universe* universe = NULL;

    try {
        universe = _universes.at(id);
    }
    catch (std::exception & e) {
        log_printf(ERROR, "Attempted to retrieve universe with id = %d which "
                 "has not been declared. Backtrace:\n%s", id, e.what());
    }

    return universe;
}


/**
 * @brief Return a pointer to a lattice from the geometry.
 * @param id the user-specified lattice (universe) ID
 * @return a pointer to the lattice object
 */
Lattice* Geometry::getLattice(short int id) {

    Lattice* lattice = NULL;
    try {
        lattice = _lattices.at(id);
    }
    catch (std::exception & e) {
        log_printf(ERROR, "Attempted to retrieve lattice with id = %d which "
                   "has not been declared. Backtrace:\n%s", id, e.what());
    }

    return lattice;
}


/**
 * @brief Add a material to the geometry.
 * @param material a pointer to a material object
 */
void Geometry::addMaterial(Material* material) {

    /* Checks if material with same id has already been added */
    if (_materials.find(material->getId()) != _materials.end())
        log_printf(ERROR, "Cannot add a second material with id = %d",
                   material->getId());

    /* Checks the number of energy groups */
    if (material->getNumEnergyGroups() == 0)
        log_printf(ERROR, "Unable to add material %d since it does not "
		 "contain any nuclear data", material->getId());
    if (_num_groups == 0)
        _num_groups = material->getNumEnergyGroups();
    else if (_num_groups != material->getNumEnergyGroups())
        log_printf(ERROR, "Unable to add material %d with %d energy groups to "
		   "the geometry which contains material(s) with %d energy"
		   "groups", material->getId(), material->getNumEnergyGroups(),
		   _num_groups);
    try {
        /* Check that the sum of the material's absorption and scattering
         * cross-sections equals its total cross-section */
        material->checkSigmaT();
        _materials.insert(std::pair<short int,Material*>(material->getId(),
                                                             material));
        log_printf(INFO, "Added material with id = %d to geometry",
                       material->getId());
    }
    catch (std::exception &e) {
        log_printf(ERROR, "Unable to add material with id = %d. Backtrace:"
                   "\n%s", material->getId(), e.what());
    }
}


/**
 * @brief Add a surface to the geometry.
 * @param surface a pointer to the surface object
 */
void Geometry::addSurface(Surface* surface) {

    /* Checks if a surface with the same id has already been added */
    if (_surfaces.find(surface->getId()) != _surfaces.end())
        return;
      //        log_printf(ERROR, "Cannot add a second surface with id = %d",
      //           surface->getId());

    else {
        try {
            _surfaces.insert(std::pair<short int, Surface*>(surface->getId(),
                                                            surface));
            log_printf(INFO, "Added surface with id = %d to geometry",
                       surface->getId());
        }
        catch (std::exception &e) {
            log_printf(ERROR, "Unable to add surface with id = %d. Backtrace:"
                       "\n%s", surface->getId(), e.what());
        }
    }

    /* Use new surface to update the boundaries of the geometry */
    switch (surface->getBoundaryType()) {
        case REFLECTIVE:
            if (surface->getXMin() < _x_min &&
                surface->getXMin() !=-std::numeric_limits<double>::infinity()) {
                _x_min = surface->getXMin();
                _left_bc = 1;
            }

            if (surface->getXMax() > _x_max &&
                surface->getXMax() != std::numeric_limits<double>::infinity()) {
                _x_max = surface->getXMax();
                _right_bc = 1;
            }

            if (surface->getYMin() < _y_min &&
                surface->getYMin() !=-std::numeric_limits<double>::infinity()) {
                _y_min = surface->getYMin();
                _bottom_bc = 1;
            }
            if (surface->getYMax() > _y_max &&
                surface->getYMax() != std::numeric_limits<double>::infinity()) {
                _y_max = surface->getYMax();
                _top_bc = 1;
            }
            break;
        case VACUUM:
            if (surface->getXMin() < _x_min && 
                surface->getXMin() !=-std::numeric_limits<double>::infinity()) {
                _x_min = surface->getXMin();
                _left_bc = 0;
            }
            if (surface->getXMax() > _x_max &&
                surface->getXMax() != std::numeric_limits<double>::infinity()) {
                _x_max = surface->getXMax();
                _right_bc = 0;
            }
            if (surface->getYMin() < _y_min &&
                surface->getYMin() !=-std::numeric_limits<double>::infinity()) {
                _y_min = surface->getYMin();
                _bottom_bc = 0;
            }
            if (surface->getYMax() > _y_max &&
                surface->getYMax() != std::numeric_limits<double>::infinity()) {
                _y_max = surface->getYMax();
                _top_bc = 0;
            }
            break;
        case BOUNDARY_NONE:
            break;
    }
        
    return;
}


/**
 * @brief Add a cell to the geometry. 
 * @details This method checks if the universe the cell is in already exists
 *           if not, it creates one and adds it to the geometry.
 * @param cell a pointer to the cell object
 */
void Geometry::addCell(Cell* cell) {

    /* Prints error msg if a cell with the same id has already been added */
    if (_cells.find(cell->getId()) != _cells.end())
        log_printf(ERROR, "Cannot add a second cell with id = %d",
                   cell->getId());

    /* Prints error msg if the cell is filled with a non-existent material */
    else if (cell->getType() == MATERIAL &&
             _materials.find(static_cast<CellBasic*>(cell)->getMaterial()) ==
             _materials.end()) {

      log_printf(ERROR, "Attempted to add cell with material with id = %d,"
                 " but material does not exist",
                 static_cast<CellBasic*>(cell)->getMaterial());
    }

    /* Set the pointers for each of the surfaces inside the cell and also
     * checks whether the cell's surfaces exist */
    //    std::map<short int, Surface*> cells_surfaces = cell->getSurfaces();
    // std::map<short int, Surface*>::iterator iter;

    /* Loop over all surfaces in the cell */
    //    for (iter = cells_surfaces.begin(); iter != cells_surfaces.end(); ++iter) {
    //        short int surface_id = abs(iter->first);

        /* Prints error msg if the surface does not exist */
    //  if (_surfaces.find(surface_id) == _surfaces.end())
    //      log_printf(ERROR, "Attempted to add cell with surface id = %d, "
    //                 "but surface does not exist", iter->first);

        /* The surface does exist, so set the surface pointer in the cell */
    //  cell->setSurfacePointer(_surfaces.at(surface_id));
    //    }

    std::map<short int, Surface*> cells_surfaces = cell->getSurfaces();
    std::map<short int, Surface*>::iterator iter;
    for (iter = cells_surfaces.begin(); iter != cells_surfaces.end(); ++iter)
        addSurface(iter->second);


    /* Insert the cell into the geometry's cell container */
    try {
        _cells.insert(std::pair<short int, Cell*>(cell->getId(), cell));
        log_printf(INFO, "Added cell with id = %d to geometry", cell->getId());
    }
    catch (std::exception &e) {
      log_printf(ERROR, "Unable to add cell with id = %d. Backtrace:"
                   "\n%s", cell->getId(), e.what());
    }

    /* Checks if the universe the cell in exists; if not, creates universe */
    if (_universes.find(cell->getUniverse()) == _universes.end()) {
        try {
            Universe* univ = new Universe(cell->getUniverse());
            addUniverse(univ);
            log_printf(INFO, "Created universe = %d", cell->getUniverse());
        }
        catch (std::exception &e) {
            log_printf(ERROR, "Unable to create a new universe with id = %d "
                       "and add it to the geometry. Backtrace:\n%s",
                       cell->getUniverse(), e.what());
        }
    }

    /* Adds the cell to the appropriate universe */
    _universes.at(cell->getUniverse())->addCell(cell);
                
    return;
}


/**
 * This method links together the pointers to the universes filling
 * CellFill class objects
 */
void Geometry::initializeCellFillPointers() {

    /* Checks if cellfill references this universe and sets its pointer */
    std::map<short int, Cell*>::iterator iter;
    CellFill* cell;
    Universe* univ;
    for (iter = _cells.begin(); iter != _cells.end(); ++iter) {

        if (iter->second->getType() == FILL) {
	    cell = static_cast<CellFill*>(iter->second);
	    univ = _universes.at(cell->getUniverseFillId());
	    cell->setUniverseFillPointer(univ);
	}
    }

    return;
}


/**
 * @brief Add a universe to the geometry.
 * @param universe a pointer to the universe object
 */
void Geometry::addUniverse(Universe* universe) {

    /* Checks if a universe with the same id has already been added */
    if (_universes.find(universe->getId()) != _universes.end())
        log_printf(ERROR, "Cannot add a second universe with id = %d",
                   universe->getId());

    /* Add the universe */
    else {
        try {
            _universes.insert(std::pair<short int,Universe*>(universe->getId(),
                                                             universe));
            log_printf(INFO, "Added universe with id = %d to geometry",
                       universe->getId());
        }
        catch (std::exception &e) {
            log_printf(ERROR, "Unable to add universe with id = %d. "
                       "Backtrace:\n%s", universe->getId(), e.what());
        }
    }

    /* Checks if any cellfill references this universe and sets its pointer */
    std::map<short int, Cell*>::iterator iter;
    for (iter = _cells.begin(); iter != _cells.end(); ++iter) {

        if (iter->second->getType() == FILL) {
            CellFill* cell = static_cast<CellFill*>(iter->second);

            if (cell->getUniverseFillId() == universe->getId()) {
                cell->setUniverseFillPointer(universe);
                int findFSRId(LocalCoords* coords);
            }
        }
    }

    return;
}


/**
 * @brief Add a lattice to the geometry. 
 * @details Adds the lattice to both the lattice and universe containers.
 * @param lattice a pointer to the lattice object
 */
void Geometry::addLattice(Lattice* lattice) {

    /* Checks whether a lattice with the same id has already been added */
    if (_lattices.find(lattice->getId()) != _lattices.end())
        log_printf(ERROR, "Cannot add a second lattice with id = %d",
                   lattice->getId());

    /* If the universes container already has a universe with the same id */
    else if (_universes.find(lattice->getId()) != _universes.end())
        log_printf(ERROR, "Cannot add a second universe (lattice) with "
                   "id = %d", lattice->getId());

    /* Sets the universe pointers for the lattice and checks if the lattice
     * contains a universe which does not exist */
    for (int i = 0; i < lattice->getNumY(); i++) {
        for (int j = 0; j < lattice->getNumX(); j++) {
            short int universe_id = lattice->getUniverses().at(i).at(j).first;

            /* If the universe does not exist */
	    if (_universes.find(universe_id) == _universes.end())
	        log_printf(ERROR, "Attempted to create lattice containing "
	                 "universe with id = %d, but universe does not exist",
	                 lattice->getUniverses().at(i).at(j).first);

            /* Set the universe pointer */
	    else
                lattice->setUniversePointer(_universes.at(universe_id));
        }
    }

    /* Add the lattice to the geometry's lattices container */
    try {
        _lattices.insert(std::pair<short int, 
                         Lattice*>(lattice->getId(), lattice));
        log_printf(INFO, "Added lattice with id = %d to geometry",
                   lattice->getId());
    }
    catch (std::exception &e) {
        log_printf(ERROR, "Unable to add lattice with id = %d. Backtrace:\n%s",
                   lattice->getId(), e.what());
    }

    /* Add the lattice to the universes container as well */
    addUniverse(lattice);
}


/**
 * @brief Find the cell that this localcoords object is in.
 * @details This method assumes that the localcoords has coordinates and 
 *          a universe id. The method will recursively find the localcoords
 *          by building a linked list of localcoords from the localcoord 
 *          passed in as an argument down to the lowest level cell found. In 
 *          the process it will set the local coordinates for each localcoord 
 *          in the linked list for the lattice or universe that it is in. If 
 *          the localcoords is outside the bounds of the geometry or on the 
 *          boundaries this method will return NULL; otherwise it will return 
 *          a pointer to the cell that the localcoords is currently in.
 * @param coords pointer to a localcoords object
 * @return returns a pointer to a cell if found, NULL if no cell found
 */
Cell* Geometry::findCell(LocalCoords* coords) {
    int universe_id = coords->getUniverse();
    Universe* univ = _universes.at(universe_id);

    if (univ->getType() == SIMPLE)
        return univ->findCell(coords, _universes);
    else
        return static_cast<Lattice*>(univ)->findCell(coords, _universes);
}


/** 
 * @brief Find the first cell of a segment with a starting point that is 
 *        represented by this localcoords object is in. 
 * @details This method assumes that the localcoords has coordinates and a 
 *          universe id. This method will move the initial starting point by 
 *          a small amount along the direction of the track in order to ensure 
 *          that the track starts inside of a distinct FSR rather than on the 
 *          boundary between two of them. The method will recursively find the
 *          localcoords by building a linked list of localcoords from the 
 *          localcoords passed in as an argument down to the lowest level 
 *          cell found. In the process it will set the local coordinates for 
 *          each localcoord in the linked list for the lattice or universe 
 *          that it is in.
 * @param coords pointer to a localcoords object
 * @param angle the angle for a trajectory projected from the localcoords
 * @return returns a pointer to a cell if found, NULL if no cell found
*/
Cell* Geometry::findFirstCell(LocalCoords* coords, double angle) {
    double delta_x = cos(angle) * TINY_MOVE;
    double delta_y = sin(angle) * TINY_MOVE;
    coords->adjustCoords(delta_x, delta_y);
    return findCell(coords);
}


/**
 * @brief Find the cell for a flat source region ID.
 * @details  This function calls the recursive function findCell with a 
 *           pointer to the base level universe 0
 * @param fsr_id a flat source region id
 * @return a pointer to the cell that this fsr is in
 */
CellBasic* Geometry::findCell(int fsr_id) {
    return static_cast<CellBasic*>(findCell(_universes.at(0), fsr_id));
}


/**
 * @brief Find the cell for an fsr_id at a certain universe level. 
 * @details This is a recursive function which is intended to be called 
 *          with the base universe 0 and a flat source region ID. It will 
 *          recursively call itself until it reaches the cell which corresponds
 *          to this flat source region.
 * @param univ a universe pointer for this flat source region's universe level
 * @param fsr_id a flat source region id
 * @return a pointer to the cell that this flat source region is in
 */
Cell* Geometry::findCell(Universe* univ, int fsr_id) {

    Cell* cell = NULL;

    /* Check if the FSR id is out of bounds */
    if (fsr_id < -1 || fsr_id > _num_FSRs)
        log_printf(ERROR, "Tried to find the cell for an fsr_id which does not "
                   "exist: %d", fsr_id);


    /* If the universe is a SIMPLE type, then find the cell the smallest fsr map
       entry that is not larger than the fsr_id argument to this function. */
    if (univ->getType() == SIMPLE) {
        std::map<short int, Cell*>::iterator iter;
        std::map<short int, Cell*> cells = univ->getCells();
        Cell* cell_min = NULL;
        int max_id = 0;
        int min_id = INT_MAX;
        int fsr_map_id;

        /* Loop over this universe's cells */
        for (iter = cells.begin(); iter != cells.end(); ++iter) {
            fsr_map_id = univ->getFSR(iter->first);
            if (fsr_map_id <= fsr_id && fsr_map_id >= max_id) {
                max_id = fsr_map_id;
                cell = iter->second;
            }
            if (fsr_map_id < min_id) {
                min_id = fsr_map_id;
                cell_min = iter->second;
            }
        }

        /* If the max_id is greater than the fsr_id, there has either been
         * an error or we are at universe 0 and need to go down one level */
        if (max_id > fsr_id) {
            if (cell_min->getType() == MATERIAL)
	       log_printf(ERROR, "Could not find cell for fsr_id = %d: "
			  "max_id(%d) > fsr_id(%d)", fsr_id, max_id, fsr_id);
            else {
                CellFill* cellfill = static_cast<CellFill*>(cell_min);
                return findCell(cellfill->getUniverseFill(), fsr_id);
            }
        }
        /* Otherwise, decrement the fsr_id and make recursive call to next
         * universe unless an error condition is met */
        else {
            fsr_id -= max_id;
            if (fsr_id == 0 && cell_min->getType() == MATERIAL)
                return cell;
            else if (fsr_id != 0 && cell_min->getType() == MATERIAL)
                log_printf(ERROR, "Could not find cell for fsr_id = %d: "
                           "fsr_id = %d and cell type = MATERIAL", 
                           fsr_id, fsr_id);
            else {
                CellFill* cellfill = static_cast<CellFill*>(cell_min);
                return findCell(cellfill->getUniverseFill(), fsr_id);
            }
        }
    }

    /* If the universe is a lattice then we find the lattice cell with the
     * smallest fsr map entry that is not larger than the fsr id argument to
     * the function. */
    else {
        Lattice* lat = static_cast<Lattice*>(univ);
        Universe* next_univ = NULL;
        int num_y = lat->getNumY();
        int num_x = lat->getNumX();
        int max_id = 0;
        int fsr_map_id;

        /* Loop over all lattice cells */
        for (int i = 0; i < num_y; i++) {
            for (int j = 0; j < num_x; j++) {
                fsr_map_id = lat->getFSR(j, i);
                if (fsr_map_id <= fsr_id && fsr_map_id >= max_id) {
                    max_id = fsr_map_id;
                    next_univ = lat->getUniverse(j, i);
                }
            }
        }

        /* If the max_id is out of bounds, then query failed */
        if (max_id > fsr_id || next_univ == NULL)
            log_printf(ERROR, "No lattice cell found for fsr = %d, max_id = "
                       "%d", fsr_id, max_id);

        /* Otherwise update fsr_id and make recursive call to next level */
        fsr_id -= max_id;
        return findCell(next_univ, fsr_id);
    }

    return cell;
}



/**
 * @brief Finds the next cell for a localcoords object along a trajectory 
 *        defined by some angle (in radians from 0 to pi). 
 * @details The method will update the localcoords passed in as an argument 
 *          to be the one at the boundary of the next cell crossed along the 
 *          given trajectory. It will do this by recursively building a linked 
 *          list of localcoords from the localcoords passed in as an argument 
 *          down to the lowest level cell found. In the process it will set 
 *          the local coordinates for each localcoords in the linked list for 
 *          the lattice or universe that it is in. If the localcoords is 
 *          outside the bounds of the geometry or on the boundaries this method  *          will return NULL; otherwise it will return a pointer to the cell
 *          that the localcoords will reach next along its trajectory.
 * @param coords pointer to a localcoords object
 * @param angle the angle of the trajectory
 * @return a pointer to a cell if found, NULL if no cell found
 */
Cell* Geometry::findNextCell(LocalCoords* coords, double angle) {

    Cell* cell = NULL;
    double dist;

    /* Find the current cell */
    cell = findCell(coords);

    /* If the current coords is not in any cell, return NULL */
    if (cell == NULL)
        return NULL;

    /* If the current coords is inside a cell, look for next cell */
    else {
        /* Check the min dist to the next surface in the current cell */
        Point surf_intersection;
        LocalCoords* lowest_level = coords->getLowestLevel();
        dist = cell->minSurfaceDist(lowest_level->getPoint(), angle,
                                    &surf_intersection);

        /* If the distance returned is not INFINITY, the trajectory will
         * intersect a surface in the cell */
        if (dist != std::numeric_limits<double>::infinity()) {
            LocalCoords test(0,0);

            /* Move LocalCoords just to the next surface in the cell plus an
             * additional small bit into the next cell */
            double delta_x = cos(angle) * TINY_MOVE;
            double delta_y = sin(angle) * TINY_MOVE;
            
            /* Copy coords to the test coords before moving it by delta and
             * finding the new cell it is in - do this for testing purposes
             * in case the new cell found is NULL or is in a new lattice cell*/
            coords->copyCoords(&test);
            coords->updateMostLocal(&surf_intersection);
            coords->adjustCoords(delta_x, delta_y);

            /* Find new cell and return it */
            cell = findCell(coords);
            
            /* Check if cell is null - this means that intersection point
             * is outside the bounds of the geometry and the old coords
             * should be restored so that we can look for the next
             * lattice cell */
            LocalCoords* test_curr = test.getLowestLevel();
            LocalCoords* coords_curr = coords->getLowestLevel();
            
            while (test_curr != NULL && test_curr->getUniverse() != 0 &&
                   coords_curr != NULL && coords_curr->getUniverse() !=0){
              
                /* Check if the next cell found is in the same lattice cell
                 * as the previous cell */
                if (coords_curr->getType() == LAT &&
                    test_curr->getType() == LAT) {

                    if (coords_curr->getLatticeX() != test_curr->getLatticeX()
                        || coords_curr->getLatticeY() != 
                        test_curr->getLatticeY()) {
                        dist = std::numeric_limits<double>::infinity();
                        break;
                    }
                }
                
                test_curr = test_curr->getPrev();
                coords_curr = coords_curr->getPrev();
            }

            /* If the cell is null then we should reset and find next lattice
             * cell rather than return this cell */
            if (cell == NULL)
                dist = std::numeric_limits<double>::infinity();

            /* If the distance is not INFINITY then the new cell found is the
             * one to return */
            if (dist != std::numeric_limits<double>::infinity()) {
                test.prune();
                return cell;
            }

            /* If the distance is not INFINITY then the new cell found is not
             * the one to return and we should move to a new lattice cell */
            else
                test.copyCoords(coords);

            test.prune();
        }

        /* If the distance returned is infinity, the trajectory will not
         * intersect a surface in the cell. We thus need to readjust to
         * the localcoord to the base universe and check whether we need
         * to move to a new lattice cell */
        if (dist == std::numeric_limits<double>::infinity()) {
          
            /* Get the lowest level localcoords in the linked list */
            LocalCoords* curr = coords->getLowestLevel();

            /* Retrace linkedlist from lowest level */
            while (curr != NULL && curr->getUniverse() != 0) {
                curr = curr->getPrev();

                /* If we reach a localcoord in a lattice, delete all lower
                 * level localcoords in linked list and break loop. */
                if (curr->getType() == LAT) {
                    curr->prune();
                    curr = NULL;
                }
            }
            
            /* Get the lowest level universe in linkedlist */
            curr = coords->getLowestLevel();

            /* Retrace through the lattices in the localcoord and check for
             * lattice cell crossings in each one. If we never find a crossing
             * and reach universe 0 the return NULL since this means we have
             * reached the edge of the geometry
             */
            while (curr->getUniverse() != 0) {
              
                /* If the lowest level localcoords is inside a lattice, find
                 * the next lattice cell */
                if (curr->getType() == LAT) {
                                        
                    short int lattice_id = curr->getLattice();
                    Lattice* lattice = _lattices.at(lattice_id);
                                        
                    cell = lattice->findNextLatticeCell(curr,angle,_universes);
                    
                    /* If the cell returned is NULL, the localcoords are outside
                     * of the current lattice, so move to a higher level lattice
                     * if there is one */
                    if (cell == NULL) {

                        /* Delete current lattice */
                        curr->getPrev()->prune();
                        
                        /* Get the lowest level localcoords in linked list */
                        curr = coords->getLowestLevel();
                                                
                        /* Retrace linkedlist from lowest level */
                        while (curr != NULL && curr->getUniverse() != 0) {
                            curr = curr->getPrev();
                            
                            /* If we reach a localcoord in a lattice, delete 
                             * all lower level localcoords in linked list 
                             * and break loop. */
                            if (curr->getType() == LAT) {
                                curr->prune();
                                curr = NULL;
                            }
                        }
                        
                        /* Get the lowest level universe in linkedlist */
                        curr = coords->getLowestLevel();
                    }

                    /* If the lowest level universe is not a lattice, then
                     * return the current cell */
                    else
                        return cell;
                }
            }
        }
    }

    /* If no cell was found, return NULL */
    return NULL;
}


/**
 * @brief Find and return the ID of the flat source region that this 
 *        localcoords object resides within.
 * @param coords a localcoords object returned
 */
int Geometry::findFSRId(LocalCoords* coords) {
    int fsr_id = 0;
    LocalCoords* curr = coords;

    while (curr != NULL) {
        if (curr->getType() == LAT) {
            Lattice* lattice = _lattices.at(curr->getLattice());
            fsr_id += lattice->getFSR(curr->getLatticeX(), curr->getLatticeY());
        }
        else if (curr->getType() == UNIV) {
            Universe* universe = _universes.at(curr->getUniverse());
            fsr_id += universe->getFSR(curr->getCell());
        }
        curr = curr->getNext();
    }

    return fsr_id;
}


void Geometry::subdivideCells() {
    std::map<short int, Universe*>::iterator iter;
    for (iter = _universes.begin(); iter != _universes.end(); ++iter)
        (*iter).second->subdivideCells();
}

/**
 * @brief Compute the number of flat source regions and nitialize an array of 
 *        FSRs and FSR maps. 
 */
void Geometry::initializeFlatSourceRegions() {

    /* Initialize pointers from fill cells to universes */
    initializeCellFillPointers();

    /* Subdivide cells into sectors and regions */
    subdivideCells();

    /* Generate flat source regions */
    Universe *univ = _universes.at(0);
    _num_FSRs = univ->computeFSRMaps();

    log_printf(NORMAL, "Number of flat source regions: %d", _num_FSRs);

    /* Allocate memory for maps between flat source regions ids and cell or
     * material ids */
    _FSRs_to_cells = new int[_num_FSRs];
    _FSRs_to_materials = new int[_num_FSRs];

    /* Load maps with cell and material ids */
    for (int r=0; r < _num_FSRs; r++) {
        CellBasic* curr=static_cast<CellBasic*>(findCell(_universes.at(0), r));
        _FSRs_to_cells[r] = curr->getId();
        _FSRs_to_materials[r] = curr->getMaterial();    
    }
}


/**
 * @brief This method creates segments within flat source regions in the 
 *        geometry for a given track. 
 * @details This method starts at the beginning of the track and finds
 *          successive intersection points with flat source regions as the 
 *          track passes through the geometry and creates segment structs 
 *          and adds them to the track.
 * @param track a pointer to a track to segmentize
 */
void Geometry::segmentize(Track* track) {

    /* Track starting point coordinates and azimuthal angle */
    double x0 = track->getStart()->getX();
    double y0 = track->getStart()->getY();
    double phi = track->getPhi();
    
    /* Length of each segment */
    FP_PRECISION segment_length;
    
    /* Use a LocalCoords for the start and end of each segment */
    LocalCoords segment_start(x0, y0);
    LocalCoords segment_end(x0, y0);
    segment_start.setUniverse(0);
    segment_end.setUniverse(0);

    /* Find the cell for the track starting point */
    Cell* curr = findFirstCell(&segment_end, phi);
    Cell* prev;

    /* If starting point was outside the bounds of the geometry */
    if (curr == NULL)
        log_printf(ERROR, "Could not find a cell containing the start point "
                   "of this track: %s", track->toString().c_str());

    /* While the segment end localcoords is still within the geometry, move
     * it to the next cell, create a new segment, and add it to the geometry */
    while (curr != NULL) {

        segment_end.copyCoords(&segment_start);

        /* Find the next cell */
        prev = curr;
        curr = findNextCell(&segment_end, phi);

        /* Find the segment length between the segments start and end points */
        segment_length = FP_PRECISION(segment_end.getPoint()
                                      ->distance(segment_start.getPoint()));

        /* Create a new segment */
        segment* new_segment = new segment;
        new_segment->_length = segment_length;
        new_segment->_material = _materials.at(static_cast<CellBasic*>(prev)
                                               ->getMaterial());

        /* Update the max and min segment lengths */
        if (segment_length > _max_seg_length)
            _max_seg_length = segment_length;
        if (segment_length < _min_seg_length)
            _min_seg_length = segment_length;

        log_printf(DEBUG, "segment start x = %f, y = %f, segment end x = %f, "
                   "y = %f", segment_start.getX(), segment_start.getY(), 
                   segment_end.getX(), segment_end.getY());

        new_segment->_region_id = findFSRId(&segment_start);
        
        /* Checks to make sure that new segment does not have the same start
         * and end points */
        if (segment_start.getX() == segment_end.getX() &&
              segment_start.getY() == segment_end.getY()) {

            log_printf(ERROR, "Created a segment with the same start and end "
                       "point: x = %f, y = %f", segment_start.getX(),
                       segment_start.getY());
        }

        /* Add the segment to the track */
        track->addSegment(new_segment);
    }

    log_printf(DEBUG, "Created %d segments for track: %s",
               track->getNumSegments(), track->toString().c_str());

    segment_start.prune();
    segment_end.prune();

    log_printf(DEBUG, "max segment length: %f", _max_seg_length);
    log_printf(DEBUG, "min segment length: %f", _min_seg_length);

    return;
}


/**
 * @brief This method is called from the Solver after fixed source iteration
 *        to compute the powers (fission rates) for each lattice cell.
 * @details This method computes both pin and assembly powers stored in
 *          arrays mapped by FSR ID in the second parameter, FSRs_to_pin_powers
 * @param FSRs_to_powers an array of the fission rate inside a given FSR
 * @param FSRs_to_pin_powers an array of the fission rate of the lattice cell
 *        this FSR is within
 */
void Geometry::computePinPowers(FP_PRECISION* FSRs_to_powers,
                                FP_PRECISION* FSRs_to_pin_powers) {

    /* Get the base universe */
    Universe* univ = _universes.at(0);

    /* Create a file prefix for the output files to store all the pin powers */
    std::string file_prefix = "PinPowers/universe0";

    /* Make call to recursive function to compute powers at each
     * level of lattice */
    computePinPowers(univ, (char*)file_prefix.c_str(), 0, FSRs_to_powers,
                     FSRs_to_pin_powers);

    return;
}


/**
 * @brief This is a recursive function which computes the powers of all of 
 *        the FSRs inside a given universe. 
 * @details This function handles both lattices and regular type universes 
 *          and saves the powers computed for each lattice cell in a file.
 * @param univ a pointer to the universe of interest
 * @param output_file_prefix the prefix for the output file to save the powers
 * @param FSR_id the FSR ID prefix from the previous level's FSR map
 * @param FSRs_to_powers array of the fission rates for each FSR
 * @param FSRs_to_pin_powers array of the fission rates for the lattice cell
 *        that each FSR is within
 */
FP_PRECISION Geometry::computePinPowers(Universe* univ, 
                                        char* output_file_prefix,
                                        int FSR_id, 
                                        FP_PRECISION* FSRs_to_powers, 
                                        FP_PRECISION* FSRs_to_pin_powers) {

    /* Power starts at 0 and is incremented for each FSR in this universe */
    FP_PRECISION power = 0;

    bool non_zero_power;

    /* If the universe is a SIMPLE type universe */
    if (univ->getType() == SIMPLE) {
        std::map<short int, Cell*> cells = univ->getCells();
        std::map<short int, short int> _region_map;
        std::vector<short int> fsr_ids;
        Cell* curr;

        /* For each of the cells inside the lattice, check if it is
         * material or fill type */
        std::map<short int, Cell*>::iterator iter;
        for (iter = cells.begin(); iter != cells.end(); ++iter) {
            curr = iter->second;

            /* If the current cell is a MATERIAL type cell, pull its
             * FSR id from the fsr map and increment the power by the
             * power for that FSR
             */
            if (curr->getType() == MATERIAL) {
                int fsr_id = univ->getFSR(curr->getId()) + FSR_id;
                fsr_ids.push_back(fsr_id);
                power += FSRs_to_powers[fsr_id];
            }

            /* If the current cell is a FILL type cell, pull its
             * FSR id from the fsr map
             */
            else {
                CellFill* fill_cell = static_cast<CellFill*>(curr);
                Universe* universe_fill = fill_cell->getUniverseFill();
                int fsr_id = univ->getFSR(curr->getId()) + FSR_id;
                
                power += computePinPowers(universe_fill, output_file_prefix, 
                                          fsr_id, FSRs_to_powers, 
                                          FSRs_to_pin_powers);
            }
        }

        /* Loop over all of the FSR ids stored for MATERIAL type cells
         * and save their pin powers in the FSRs_to_pin_powers map */
        for (int i=0; i < (int)fsr_ids.size(); i++) {
            int fsr_id = fsr_ids.at(i);
            FSRs_to_pin_powers[fsr_id] = power;
        }
    }

    /* If the universe is a LATTICE type universe */
    else {
        Lattice* lattice = static_cast<Lattice*>(univ);
        Universe* curr;
        int num_x = lattice->getNumX();
        int num_y = lattice->getNumY();
        int fsr_id;
        FP_PRECISION cell_power = 0;

        /* Create an output file to write this lattice's pin powers to within
         * a new directory called PinPowers */
        mkdir("PinPowers", S_IRWXU);
        std::stringstream output_file_name;
        output_file_name << output_file_prefix <<
            "_lattice" << lattice->getId() << "_power.txt";
        FILE* output_file = fopen(output_file_name.str().c_str(), "w");

        non_zero_power = false;

        /* Loop over all lattice cells in this lattice */
        for (int i = num_y-1; i > -1; i--) {
            for (int j = 0; j < num_x; j++) {
            
                /* Get a pointer to the current lattice cell */
                curr = lattice->getUniverse(j, i);

                /* Get the FSR id prefix for this lattice cell */
                fsr_id = lattice->getFSR(j, i) + FSR_id;

                /* Create an output filename for this cell's power */
                std::stringstream file_prefix;
                file_prefix << output_file_prefix << "_lattice" <<
                    lattice->getId() << "_x" << j << "_y" << i;

                /* Find this lattice cell's power */
                cell_power = computePinPowers(curr,
                                              (char*)file_prefix.str().c_str(),
                                              fsr_id, FSRs_to_powers, 
                                              FSRs_to_pin_powers);

                /* Write this lattice cell's power to the output file */
                fprintf(output_file, "%f, ", cell_power);
                
                power += cell_power;
                
                /* Check if a nonzero power has been computed */
                if (power > 0.0)
                    non_zero_power = true;
            }
            /* Move to the next line in the output file */
            fprintf(output_file, "\n");
        }

        fclose(output_file);
        
        /* Delete this output file if none of the powers were nonzero */
        if (!non_zero_power)
            remove(output_file_name.str().c_str());
        
        }

    return power;
}



/**
 * @brief Converts this geometry's attributes to a character array.
 * @return a character array of this geometry's attributes
 */
std::string Geometry::toString() {

    std::stringstream string;
    std::map<short int, Material*>::iterator iter1;
    std::map<short int, Surface*>::iterator iter2;
    std::map<short int, Cell*>::iterator iter3;
    std::map<short int, Universe*>::iterator iter4;
    std::map<short int, Lattice*>::iterator iter5;

    string << "Geometry: width = " << getWidth() << ", height = " <<
        getHeight() << ", Bounding Box: ((" << _x_min << ", " <<
        _y_min << "), (" << _x_max << ", " << _y_max << ")";

    string << "\n\tMaterials:\n\t\t";
    for (iter1 = _materials.begin(); iter1 != _materials.end(); ++iter1)
        string << iter1->second->toString() << "\n\n\t\t";

    string << "\n\tSurfaces:\n\t\t";
    for (iter2 = _surfaces.begin(); iter2 != _surfaces.end(); ++iter2)
        string << iter2->second->toString() << "\n\t\t";

    string << "\n\tCells:\n\t\t";
    for (iter3 = _cells.begin(); iter3 != _cells.end(); ++iter3)
        string << iter3->second->toString() << "\n\t\t";

    string << "\n\tUniverses:\n\t\t";
    for (iter4 = _universes.begin(); iter4 != _universes.end(); ++iter4)
        string << iter4->second->toString() << "\n\t\t";

    string << "\n\tLattices:\n\t\t";
    for (iter5 = _lattices.begin(); iter5 != _lattices.end(); ++iter5)
        string << iter5->second->toString()  << "\n\t\t";

    std::string formatted_string = string.str();
    formatted_string.erase(formatted_string.end()-3);

    return formatted_string;
}


/**
 * @brief Prints a string representation of all of the geometry's objects to
 *        the console.
 */
void Geometry::printString() {
    printf("[  RESULT ]  %s", toString().c_str());
}