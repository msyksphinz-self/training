/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.minimips.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.minimips.PE;
import ru.ispras.microtesk.model.minimips.TempVars;

import static ru.ispras.microtesk.model.minimips.TypeDefs.*;

public final class instruction extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "instruction",
          instruction.class,
          null
          );
      addArgument("command", Operations.INFO);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final IsaPrimitive command = (IsaPrimitive) getArgument("command", args);

      return new instruction(command);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final IsaPrimitive command;

  public instruction(final IsaPrimitive command) {
    assert Operations.INFO.isSupported(command);

    this.command = command;

    addArgument("command", command);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return command.text(vars__);
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return command.image(vars__);
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    pe__.GPR.access(0).store(Data.valueOf(WORD, 0x0));
    if (pe__.CIA.access().load().equals(pe__.CIA_COPY.access().load())) {
      vars__.is_delay_slot.access().store(pe__.BRANCH.access().load());
      vars__.jump_address.access().store(pe__.JMPADDR.access().load());
    } else {
      vars__.is_delay_slot.access().store(Data.valueOf(BIT, 0x0));
      vars__.jump_address.access().store(Data.valueOf(WORD, 0x0));
    }
    pe__.BRANCH.access().store(Data.valueOf(BIT, 0x0));
    pe__.JMPADDR.access().store(Data.valueOf(WORD, 0x0));
    command.execute(pe__, vars__);
    if (vars__.is_delay_slot.access().load().equals(Data.valueOf(BIT, 0x1)) && pe__.BRANCH.access().load().equals(Data.valueOf(BIT, 0x1))) {
      Execution.unpredicted();
    }
    if (vars__.is_delay_slot.access().load().equals(Data.valueOf(BIT, 0x1))) {
      pe__.CIA.access().store(vars__.jump_address.access().load());
    } else {
      pe__.CIA.access().store(pe__.CIA.access().load().add(Data.valueOf(WORD, 0x4)));
    }
    pe__.CIA_COPY.access().store(pe__.CIA.access().load());
  }
}