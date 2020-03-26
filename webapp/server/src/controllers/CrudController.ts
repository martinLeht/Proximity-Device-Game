import { Request, Response } from "express";

// Abstract class to define a controller with CRUD functionalities
export abstract class CrudController {

    public abstract create(req: Request, res: Response): void;
    public abstract read(req: Request, res: Response): void;
    public abstract update(req: Request, res: Response): void;
    public abstract delete(req: Request, res: Response): void;
}