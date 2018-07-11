<?php

namespace App\Http\Controllers;

use App\Ticket;
use App\Bug;
use App\Module;
use App\DataTypes;
use App\Description;
use App\User;
use DB;
use Illuminate\Http\Request;

class TicketController extends Controller
{
    public function index()
    {
        $tickets = Ticket::all();
        return view('tickets.tickets', compact('tickets'));
    }


    public function create() {
        $bugs = DB::table('bugs')->
        where('id_patch', null)->
        join('modules', 'bugs.id_modul', '=', 'modules.id_modul')->
        get();
        $modules = Module::all();
        $bug_types = DataTypes::$typ_bugu;
        $vulner  = DataTypes::$vulner;
        $severity = DataTypes::$zavaznost;
        return view('tickets.create', compact('bugs', 'modules', 'bug_types', 'vulner', 'severity'));
    }


    public function show($id_ticket)
    {

        $ticket = Ticket::where('id_ticket',$id_ticket)->first();
        $bugs = Description::where('id_ticket', $id_ticket)->
        join('bugs', 'descriptions.id_bug', '=', 'bugs.id_bug')->
        join('modules', 'bugs.id_modul', '=', 'modules.id_modul')->
        get();
        $users = User::all();

        $ticket_types = DataTypes::$ticket_type;


        return view('tickets.ticket', compact('ticket', 'ticket_types', 'bugs', 'users'));
    }

    public function store(Request $request) {

        $bugs = explode(',', $request->id_bug);

        if ($request->id_bug)
            $validate = $request->validate([
                'popis_ticketu' => 'required|max:255',
                'id_bug' => 'required',
            ]);
        else
            $validate = $request->validate([
                'popis_ticketu' => 'required|max:255',
                'popis_bugu' => 'required|max:255',
                'zavaznost' => 'required',
            ]);

        $module = Module::where('meno', $request->meno_modulu)->first();

        DB::table('tickets')->insert([
            'stav' => 'New',
            'name_vytvoreny' => $request->name_vytvoreny,
            'popis' => $request->popis_ticketu
        ]);

        $id_ticket = DB::table('tickets')->orderBy('id_ticket', 'desc')->first()->id_ticket;

        if (!$request->id_bug) {
            DB::table('bugs')->insert([
                'typ' => $request->typ,
                'zavaznost' => $request->zavaznost,
                'id_modul' => $module->id_modul,
                'popis_bugu' => $request->popis_bugu
            ]);
            $request->id_bug = DB::table('bugs')->orderBy('id_bug', 'desc')->first()->id_bug;
            DB::table('descriptions')->insert([
                'id_ticket' => $id_ticket,
                'id_bug' => $request->id_bug,
            ]);
        } else {
            foreach ($bugs as $bug) {
                DB::table('descriptions')->insert([
                    'id_ticket' => $id_ticket,
                    'id_bug' => $bug
                ]);
            }
        }

        if ($request->miera_nebezpecenstva)
            DB::table('vulnerabilities')->insert([
                'id_vulnerability' => $request->id_bug,
                'miera_nebezpecenstva' => $request->miera_nebezpecenstva
            ]);

        $new_ticket = Ticket::get()->last();

        return redirect('/tickets/'.$new_ticket->id_ticket);
    }

    public function store_ticket(Request $request){

        if($request->accept) {
            DB::table('tickets')->where('id_ticket', $request->id)->update([
                'name_prideleny' => $request->accept,
                'stav' => 'Assigned'
            ]);
        } elseif ($request->reject){
            DB::table('tickets')->where('id_ticket', $request->id)->update([
                'name_prideleny' => null,
                'stav' => 'Open',
            ]);
        } else {
            DB::table('tickets')->where('id_ticket', $request->id)->update([
                'stav' => $request->state,
                'popis' => $request->desc,
            ]);
        }
        return redirect('/tickets/'.$request->id);
    }


}
